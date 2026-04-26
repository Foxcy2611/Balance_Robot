# 🤖 ESP32 Self-Balancing Robot (Xe Cân Bằng 2 Bánh)

![Project Status](https://img.shields.io/badge/Status-In_Progress-orange)
![Platform](https://img.shields.io/badge/Platform-ESP32-blue)
![Framework](https://img.shields.io/badge/Framework-PlatformIO-lightgrey)

## 📝 Giới thiệu Dự án
Dự án phát triển xe tự cân bằng 2 bánh sử dụng vi điều khiển ESP32. Khác với các project sử dụng thư viện có sẵn (high-level), project này tập trung vào việc tự xây dựng các module driver mức thanh ghi (bare-metal style) để tối ưu hóa hiệu suất và vòng lặp thuật toán PID.

**Tác giả:** Nguyễn Ngọc Chiến - B23DCVT061
**Đơn vị:** Học viện Công nghệ Bưu chính Viễn thông (PTIT)

## ✨ Tính năng chính (Features)
- ⚖️ **Cân bằng tự động:** Sử dụng thuật toán PID (Proportional-Integral-Derivative) để giữ thăng bằng dựa trên góc nghiêng.
- 📐 **Xử lý tín hiệu cảm biến:** Đọc dữ liệu thô từ MPU6050 qua I2C và sử dụng bộ lọc bù (Complementary Filter) để khử nhiễu, lấy góc Pitch chính xác.
- ⚙️ **Đọc Encoder tốc độ cao:** Tận dụng bộ ngoại vi đếm xung phần cứng **PCNT** của ESP32 (chế độ đếm 4X) để đọc encoder mà không làm nghẽn CPU.
- ⚡ **Điều khiển động cơ mượt mà:** Sử dụng ngoại vi **LEDC** của ESP32 để băm xung PWM điều khiển L298N.
- 📱 **Điều khiển từ xa:** Tích hợp Bluetooth Serial để nhận lệnh điều hướng (Tiến, Lùi, Trái, Phải) từ App điện thoại.

## 🛠️ Phần cứng (Hardware)
- **MCU:** ESP32 Dev Module
- **Cảm biến:** MPU6050 (Gia tốc kế & Con quay hồi chuyển)
- **Driver Động cơ:** Mạch cầu H L298N
- **Động cơ:** 2x Động cơ DC giảm tốc tích hợp Encoder (Pha A, Pha B)
- **Nguồn:** Pin LiPo 2S/3S hoặc 2x 18650 (Kèm mạch hạ áp Buck DC-DC cho ESP32)

## 📁 Cấu trúc Thư mục (Project Structure)
Project được tổ chức theo mô hình Modular trên PlatformIO:

```text
Balance_Robot/
├── include/
├── lib/
│   ├── Algor/                # Chứa thuật toán điều khiển
│   │   ├── PID_Control.cpp
│   │   ├── PID_Control.h
│   │   ├── Remote_Control.cpp
│   │   └── Remote_Control.h
│   └── Sensor/               # Chứa driver giao tiếp phần cứng
│       ├── motor.cpp
│       ├── motor.h
│       ├── mpu6050.cpp
│       └── mpu6050.h
├── src/
│   └── main.cpp              # Vòng lặp điều khiển chính (Control Loop)
└── platformio.ini            # Cấu hình môi trường build
