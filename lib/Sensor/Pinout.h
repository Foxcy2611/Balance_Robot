#ifndef __PINOUT__H
#define __PINOUT__H

/*---------- MPU6050 ----------*/
#define I2C_SDA        21
#define I2C_SCL        22

/*----------- L298n -----------*/
// ========= MOTOR TRÁI =========
#define L_IN1          14  // Băm xung chạy tiến
#define L_IN2          27  // Băm xung chạy lùi
#define L_ENC_A        32  // Xung chính đếm Encoder
#define L_ENC_B        33  // Xung phụ phân biệt chiều

// ========= MOTOR PHẢI =========
#define R_IN1          12
#define R_IN2          13
#define R_ENC_A        35 
#define R_ENC_B        34

/*---------- THÔNG SỐ BĂM XUNG (LEDC PWM) ----------*/
#define PWM_FREQ       10000
#define PWM_RES        8

/*----------- 4 KÊNH PWM RIÊNG BIỆT CHO 4 CHÂN IN -----------*/
#define L_IN1_CHANNEL  0
#define L_IN2_CHANNEL  1

#define R_IN1_CHANNEL  2
#define R_IN2_CHANNEL  3

/*
    Không cần dùng ENA, ENB mà sử dụng chính thằng IN
    Vì IN1/IN2 quyết định chiều đi, nên khi muốn đi tiến/lùi, băm trực tiếp và chân IN đó
    Vdu: Muốn đi tiến, băm xung vào IN1, ko băm gì IN2

*/

#endif