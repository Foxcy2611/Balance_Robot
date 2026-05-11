#include <Arduino.h>
#include <motor.h>
#include <mpu6050.h>
#include "PID_Control.h"

void setup() {
    Serial.begin(115200);
    delay(1000);

    Motor_Init();
    MPU6050_Init();
    
    Serial.println("De im xe de Calibrate Gyro trong 3 giay...");
    // BẮT BUỘC: Không chạm vào xe lúc này
    Balance_Init(); 
    
    // Setpoint vừa tìm được
    Balance_SetTarget(-2.3f); 
    Serial.println("=== BALANCE BOT START ===");
}

void loop() {
    static unsigned long prev_us = micros();
    unsigned long now = micros();
    
    // Non-blocking delay 5ms (200Hz)
    if (now - prev_us >= 5000) {
        prev_us = now;

        // PID tự tính dt bên trong
        float pid_out = Balance_Complete_PID();

        // Cấp tốc độ cho motor
        // Nếu xe ngã tới mà bánh chạy lùi -> Đổi thành Motor_SetSpeed(-pid_out, -pid_out);
        Motor_SetSpeed(-(int)pid_out, -(int)pid_out);
    }
}