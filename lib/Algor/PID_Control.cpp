#include "PID_Control.h"

/* ----- Complementary Filter ----- */
// Bằng cách trộn dữ liệu giữa G và A để tính toán chính xác và nhanh hơn
// +) Accel: Đo góc nghiêng dựa trọng lực 
// => Chính xác lâu dài nhưng dễ nhiễu khi rung động
// +) Gyro: Đo tốc độ quay
// => Mượt nhưng ngắn hạn, dễ trôi theo tgian

// Công thức: o_pitch = a . (o_pitch + w_gyro x dt) + (1 - a) x o_accel
// Angle: Góc, pitch: độ nghiêng
// a: Hệ số tin cậy của gyro (Thường là 0.98)
// w_gyro: Vận tốc góc gyro
// ==> o_pitch = ? làm input cho PID

// Theo hình, trục Y song song trục bánh, trục X sẽ chỉ về mũi xe

/* ----- Complementary Filter ----- */
static float anphal = 0.98f;

/* ----- Hệ số PID ----- */
static float Kp = 17.0f;
static float Ki = 0.0f;
static float Kd = 1.5f;
static float DEAD_BAND = 70.0f;

/* ----- Trạng thái nội bộ -----*/
static float o_pitch = 0.0f;
static float setpoint = 0.0f;

static float integral = 0.0f;
static float d_filtered = 0.0f; // D sau low-pass

static float gyro_offset_y = 0.0f; // Calibration bias
static unsigned long prev_us = 0; // dt thực tế

/* --- LOW PASS cho D */
// Càng nhỏ thì càng mượt nhưng lag
#define LPF_D 0.15f

void Balance_Init(){
    integral   = 0.0f;
    o_pitch    = 0.0f;
    d_filtered = 0.0f;
    prev_us    = micros();

    Calibrate_Gyro();
}

void Calibrate_Gyro(){
    int16_t ax, ay, az;
    int16_t gx, gy, gz;

    long sum = 0;
    Serial.println("Let Calib gyro... Keep still !");

    for(int i = 1 ; i <= 500 ; i++){
        MPU6050_ReadGyro(&gx, &gy, &gz);
        sum += gy;
        delay(2);
    }

    gyro_offset_y = (float)sum / 500.0f;
    Serial.printf("Gyro offset Y = %.2f\n", gyro_offset_y);
}

void Balance_SetTarget(float angle_deg){
    setpoint = angle_deg;
    // Test: -2.3f
}

float Balance_GetPitch(float dt_actual){
    int16_t ax, ay, az;
    int16_t gx, gy, gz;
    
    MPU6050_ReadAccel(&ax, &ay, &az);
    MPU6050_ReadGyro(&gx, &gy, &gz);

    float accel_pitch = atan2f((float)ax, (float)az) * 180.0f / PI;

    float gyro_rate = ((float)gy - gyro_offset_y) / 131.0f;

    o_pitch = anphal * (o_pitch + gyro_rate * dt_actual) + (1- anphal) * accel_pitch;

    // Bộ lọc trung bình mũ, làm mềm tín hiệu D
    // y[n] = a . x[n] + (1 - a) . y[n - 1]
    d_filtered = LPF_D * gyro_rate + (1 - LPF_D) * d_filtered;

    Serial.printf(">Pitch:%.2f\n",   o_pitch);
    Serial.printf(">Accel:%.2f\n",   accel_pitch);
    Serial.printf(">Gyro:%.2f\n",    gyro_rate);

    return o_pitch;
}

/*
    Turning PID: Ban đầu set tất cả = 0
    - P: 
    + Xe không phản ứng: Kp quá nhỏ
    + Xe cố đứng nhưng dao động sin đều Kp: Vừa tới
    + Dao động ngày càng mạnh: Kp quá lớn

    - D:
    + Dao động vẫn còn: Kd nhỏ
    + Xe đứng khá ổn: Kd đúng vùng
    + Bánh lăn nhẹ, motor nóng, đặt phát ngã luôn: Kd lớn

    - I: rất nhỏ

*/

float Balance_Complete_PID(){
    // Đo dt thay vì dùng hằng số
    unsigned long now = micros();
    float dt = (float)(now - prev_us) / 1000000.0f;
    prev_us = now;

    if(dt <= 0.0f || dt > 0.02f) return 0.0f;

    float angle = Balance_GetPitch(dt);

    if(fabsf(angle) > 45.0f){
        integral = 0.0f;
        d_filtered = 0.0f;
        return 0.0f;
    }

    float err = setpoint - angle;

    if(fabsf(err) < 15.0f){
        integral += err * dt;
        integral = constrain(integral, -50.0f, 50.0f);
    } else {
        integral = 0.0f;
    }

    float output = Kp * err + Ki * integral + Kd * (-d_filtered);
    // Vì setpoint - angle mà setpoint muốn là = 0
    // Nên đạo hàm D = -(angel), cần đổi dấu về +
    if(output > 0.5f) output += DEAD_BAND;
    else if(output < -0.5f) output -= DEAD_BAND;

    output = constrain(output, -255, 255);

    return output;
}