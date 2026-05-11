#include <motor.h>

void Motor_Init(){
    ledcSetup(L_IN1_CHANNEL, PWM_FREQ, PWM_RES);
    ledcSetup(L_IN2_CHANNEL, PWM_FREQ, PWM_RES);
    
    ledcSetup(R_IN1_CHANNEL, PWM_FREQ, PWM_RES);
    ledcSetup(R_IN2_CHANNEL, PWM_FREQ, PWM_RES);

    ledcAttachPin(L_IN1, L_IN1_CHANNEL);
    ledcAttachPin(L_IN2, L_IN2_CHANNEL);

    ledcAttachPin(R_IN1, R_IN1_CHANNEL);
    ledcAttachPin(R_IN2, R_IN2_CHANNEL);

    /* ----- Pause cho động cơ dừng -----*/
    ledcWrite(L_IN1_CHANNEL, 0);
    ledcWrite(L_IN2_CHANNEL, 0);
    ledcWrite(R_IN1_CHANNEL, 0);
    ledcWrite(R_IN2_CHANNEL, 0);

    /* ------- Bánh trái -------*/
    // Channel 0: Pha A làm xung, B làm tín hiệu
    pcnt_config_t pcnt_config_L_Ch0 = {
        .pulse_gpio_num = L_ENC_A, // Chân nhận xung chính
        .ctrl_gpio_num = L_ENC_B, // Chân kiếm soát chiều quay

        /* Logic phân biệt chiều quay */
        // Nếu B mức cao HIGH: Giữ cách đếm
        .lctrl_mode = PCNT_MODE_KEEP,
        // Nếu B mức cao HIGH: Đảo cách đếm
        .hctrl_mode = PCNT_MODE_REVERSE,

        /* Hành vi khi có xung chân chính (Pha A) */
        .pos_mode = PCNT_COUNT_INC, // Cạnh lên (0 -> 1) thì cộng
        .neg_mode = PCNT_COUNT_DEC, // Cạnh xuống (0-> 1) thì trừ

        /* Giới hạn đếm (int16_t) */
        .counter_h_lim = 32767,
        .counter_l_lim = -32768,

        .unit = PCNT_UNIT_0,
        .channel = PCNT_CHANNEL_0,
    };
    pcnt_unit_config(&pcnt_config_L_Ch0); 

    // Channel 1: Pha B làm xung, A làm tín hiệu
    pcnt_config_t pcnt_config_L_Ch1 = {
        .pulse_gpio_num = L_ENC_B,
        .ctrl_gpio_num = L_ENC_A,

        .lctrl_mode = PCNT_MODE_REVERSE,
        .hctrl_mode = PCNT_MODE_KEEP,

        .pos_mode = PCNT_COUNT_INC,
        .neg_mode = PCNT_COUNT_DEC,

        .counter_h_lim = 32767,
        .counter_l_lim = -32768,

        .unit = PCNT_UNIT_0,
        .channel = PCNT_CHANNEL_1,
    };
    pcnt_unit_config(&pcnt_config_L_Ch1);

    pcnt_counter_pause(PCNT_UNIT_0); // Tạm dừng để reset
    pcnt_counter_clear(PCNT_UNIT_0); // Đưa về 0
    pcnt_counter_resume(PCNT_UNIT_0); // Bắt đầu chạy ngầm

    /* Bánh phải UNIT 1 */
    // Channel 0 bánh phải
    pcnt_config_t pcnt_config_R_Ch0 = pcnt_config_L_Ch0; // Copy cấu hình bánh trái
    pcnt_config_R_Ch0.pulse_gpio_num = R_ENC_A;      // Đổi chân
    pcnt_config_R_Ch0.ctrl_gpio_num = R_ENC_B;       // Đổi chân
    pcnt_config_R_Ch0.unit = PCNT_UNIT_1;            // Đổi bộ đếm sang số 1
    pcnt_unit_config(&pcnt_config_R_Ch0);
    
    // Channel 1 bánh phải
    pcnt_config_t pcnt_config_R_Ch1 = pcnt_config_L_Ch1; 
    pcnt_config_R_Ch1.pulse_gpio_num = R_ENC_B;      
    pcnt_config_R_Ch1.ctrl_gpio_num = R_ENC_A;       
    pcnt_config_R_Ch1.unit = PCNT_UNIT_1;   
    pcnt_unit_config(&pcnt_config_R_Ch1);

    pcnt_counter_pause(PCNT_UNIT_1);
    pcnt_counter_clear(PCNT_UNIT_1);
    pcnt_counter_resume(PCNT_UNIT_1);
}

void Motor_SetSpeed(int speed_L, int speed_R){
    /* --- Limit PWM ---*/
    speed_L = constrain(speed_L, -255, 255);
    speed_R = constrain(speed_R, -255, 255);

    /* Xử lý Motor trái */
    if(speed_L > 0){
        // Chạy tiến: Băm IN1, tắt IN2
        ledcWrite(L_IN1_CHANNEL, speed_L);
        ledcWrite(L_IN2_CHANNEL, 0);
    } else if(speed_L < 0) {
        ledcWrite(L_IN1_CHANNEL, 0);
        ledcWrite(L_IN2_CHANNEL, -speed_L);
    } else {
        ledcWrite(L_IN1_CHANNEL, 0);
        ledcWrite(L_IN2_CHANNEL, 0);
    }

    /* Xử lý Motor phải */
    if(speed_R > 0){
        // Chạy tiến: Băm IN1, tắt IN2
        ledcWrite(R_IN1_CHANNEL, speed_R);
        ledcWrite(R_IN2_CHANNEL, 0);
    } else if(speed_R < 0) {
        ledcWrite(R_IN1_CHANNEL, 0);
        ledcWrite(R_IN2_CHANNEL, -speed_R);
    } else {
        ledcWrite(R_IN1_CHANNEL, 0);
        ledcWrite(R_IN2_CHANNEL, 0);
    }
}

int32_t Encoder_GetCount_L(){
    int16_t count;
    pcnt_get_counter_value(PCNT_UNIT_0, &count);
    
    return (int32_t)count;
}

int32_t Encoder_GetCount_R(){
    int16_t count;
    pcnt_get_counter_value(PCNT_UNIT_1, &count);

    return (int32_t)count;
}
