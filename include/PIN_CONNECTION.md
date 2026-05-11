# 📍 Sơ Đồ Kết Nối Chân ESP32

Tài liệu này mô tả chi tiết các kết nối chân điều khiển giữa ESP32 và các thiết bị ngoại vi cho robot cân bằng tự động.

---

## 📋 Tổng Quan Hệ Thống

```
┌─────────────────┐
│     ESP32       │
│   (DevKit C)    │
└────────┬────────┘
         │
    ┌────┼────┬──────────┬─────────────┐
    │    │    │          │             │
    ▼    ▼    ▼          ▼             ▼
 MOTOR  ENC  IMU      SERIAL    (SPI SD Card)
```

---

## 🔧 Chi Tiết Kết Nối Chân

### 1️⃣ **Động Cơ (Motor Driver)**

| Chức Năng | Chân ESP32 | GPIO | Mô Tả |
|-----------|-----------|------|-------|
| **Motor Phải - IN1** | GPIO 12 | 12 | Điều khiển chiều quay động cơ phải |
| **Motor Phải - IN2** | GPIO 13 | 13 | Điều khiển chiều quay động cơ phải |
| **Motor Trái - IN3** | GPIO 14 | 14 | Điều khiển chiều quay động cơ trái |
| **Motor Trái - IN4** | GPIO 27 | 27 | Điều khiển chiều quay động cơ trái |

**Sơ đồ kết nối Motor:**
```
┌──────────────────────────────────────┐
│        Motor Driver Module           │
├──────────────────────────────────────┤
│                                      │
│  IN1 (GPIO 12) ──► Motor Right       │
│  IN2 (GPIO 13) ──► Motor Right       │
│                                      │
│  IN3 (GPIO 14) ──► Motor Left        │
│  IN4 (GPIO 27) ──► Motor Left        │
│                                      │
│  GND ──────────► ESP32 GND           │
│  VCC ──────────► Power Supply        │
│                                      │
└──────────────────────────────────────┘
```

---

### 2️⃣ **Cảm Biến Encoder (Tốc độ)**

| Chức Năng | Chân ESP32 | GPIO | Mô Tả |
|-----------|-----------|------|-------|
| **Encoder Trái - Phase A** | GPIO 32 | 32 | Phát hiện tốc độ motor trái (PCNT Unit 0) |
| **Encoder Trái - Phase B** | GPIO 33 | 33 | Phát hiện tốc độ motor trái (PCNT Unit 0) |
| **Encoder Phải - Phase A** | GPIO 35 | 35 | Phát hiện tốc độ motor phải (PCNT Unit 1) |
| **Encoder Phải - Phase B** | GPIO 34 | 34 | Phát hiện tốc độ motor phải (PCNT Unit 1) |

**Sơ đồ kết nối Encoder:**
```
┌──────────────────────────────────────┐
│      Rotary Encoder Module           │
├──────────────────────────────────────┤
│                                      │
│  Left Encoder:                       │
│    Phase A (GPIO 32) ──► PCNT Unit 0 │
│    Phase B (GPIO 33) ──► PCNT Unit 0 │
│    GND ──────────► ESP32 GND         │
│                                      │
│  Right Encoder:                      │
│    Phase A (GPIO 35) ──► PCNT Unit 1 │
│    Phase B (GPIO 34) ──► PCNT Unit 1 │
│    GND ──────────► ESP32 GND         │
│                                      │
│  VCC ──────────► Power Supply (5V)   │
│                                      │
└──────────────────────────────────────┘
```

**Ghi chú Encoder:**
- Sử dụng **PCNT (Pulse Counter)** của ESP32 để đếm xung
- Motor Trái: `PCNT_UNIT_0`
- Motor Phải: `PCNT_UNIT_1`

---

### 3️⃣ **Cảm Biến IMU (MPU6050)**

| Chức Năng | Chân ESP32 | Protocol | Mô Tả |
|-----------|-----------|----------|-------|
| **SDA (Data)** | GPIO 21 | I2C | Dữ liệu I2C |
| **SCL (Clock)** | GPIO 22 | I2C | Xung I2C |
| **VCC** | 3.3V | - | Nguồn điện |
| **GND** | GND | - | Đất chung |
| **AD0 (Địa chỉ)** | GND | - | Địa chỉ I2C: 0x68 |

**Sơ đồ kết nối MPU6050:**
```
┌──────────────────────────────────────┐
│         MPU6050 (IMU Module)         │
├──────────────────────────────────────┤
│                                      │
│  SDA (GPIO 21) ◄──► I2C Data Line    │
│  SCL (GPIO 22) ◄──► I2C Clock Line   │
│                                      │
│  AD0 ──────────► GND                 │
│    ➜ I2C Địa chỉ: 0x68              │
│                                      │
│  VCC ──────────► ESP32 3.3V          │
│  GND ──────────► ESP32 GND           │
│                                      │
│  Sensors:                            │
│  • Accelerometer (ax, ay, az)        │
│  • Gyroscope (gx, gy, gz)            │
│  • Temperature                       │
│                                      │
└──────────────────────────────────────┘
```

**Ghi chú I2C:**
- **Địa chỉ I2C:** `0x68`
- **Tần số Clock:** Tùy theo cấu hình (thường 100 kHz hoặc 400 kHz)
- Pull-up resistor có thể được yêu cầu nếu không có sẵn

---

## 📊 Tóm Tắt Chân GPIO

### GPIO được sử dụng:

```
ESP32 GPIO Layout
═══════════════════════════════════════════

┌─────────────────────────────────────┐
│          GPIO MAPPING               │
├─────────────────────────────────────┤
│                                     │
│  I2C:                               │
│    GPIO 21 (SDA) ──── MPU6050       │
│    GPIO 22 (SCL) ──── MPU6050       │
│                                     │
│  Motor Control:                     │
│    GPIO 12 (Right IN1) ── Motor     │
│    GPIO 13 (Right IN2) ── Motor     │
│    GPIO 14 (Left IN3)  ── Motor     │
│    GPIO 27 (Left IN4)  ── Motor     │
│                                     │
│  Encoder (PCNT):                    │
│    GPIO 32 (Left A)  ── PCNT_UNIT_0 │
│    GPIO 33 (Left B)  ── PCNT_UNIT_0 │
│    GPIO 34 (Right B) ── PCNT_UNIT_1 │
│    GPIO 35 (Right A) ── PCNT_UNIT_1 │
│                                     │
│  Reserved:                          │
│    GPIO 5  (VSPI CLK)               │
│    GPIO 18 (VSPI CLK)               │
│    GPIO 23 (VSPI MOSI)              │
│    GPIO 19 (VSPI MISO)              │
│    GPIO 15 (VSPI CS)                │
│                                     │
└─────────────────────────────────────┘
```

---

## ⚙️ Cấu Hình Phần Cứng

### Yêu cầu Nguồn:
- **ESP32:** 5V (USB hoặc Power Supply)
- **Motor Driver:** 5V - 12V (tùy theo động cơ)
- **MPU6050:** 3.3V
- **Encoder:** 5V

### Khuyến Nghị:
- ✅ Thêm **Pull-up resistor 10kΩ** cho SDA và SCL (nếu không có trên module)
- ✅ Thêm **Capacitor 0.1µF** gần VCC của MPU6050
- ✅ Sử dụng **dây nối ngắn** để giảm nhiễu, đặc biệt cho Encoder
- ✅ Tất cả thiết bị ngoại vi phải **chia chung GND** với ESP32

---

## 📝 Mã Khởi Tạo

### Motor Initialization:
```cpp
Motor motor;
motor.Begin(
    12,  // RIGHT_MOTOR_IN1
    13,  // RIGHT_MOTOR_IN2
    14,  // LEFT_MOTOR_IN3
    27   // LEFT_MOTOR_IN4
);
```

### Encoder Initialization:
```cpp
Encoder encoder_left;
Encoder encoder_right;

encoder_left.Begin(32, 33, PCNT_UNIT_0);   // GPIO 32, 33 - Phase A, B
encoder_right.Begin(35, 34, PCNT_UNIT_1);  // GPIO 35, 34 - Phase A, B
```

### MPU6050 Initialization:
```cpp
MPU6050 imu;
imu.Begin(Wire);  // I2C Address: 0x68
imu.CalibrateGyro(500);
```

---

## 🔌 Sơ Đồ Nối Toàn Bộ Hệ Thống

```
                    ┌─────────────────────────┐
                    │       ESP32             │
                    │                         │
         ┌──────────┤ GPIO 21 ─────┐          │
         │          │ GPIO 22 ─────┤──I2C─┐  │
         │          │               │      │  │
         │          │ GPIO 12 ────┐ │      │  │
         │          │ GPIO 13 ─┐  │ │      │  │
         │          │ GPIO 14 ─┤  │ │      │  │
         │          │ GPIO 27 ────┤ │      │  │
         │          │               │      │  │
         │          │ GPIO 32 ────┐ │      │  │
         │          │ GPIO 33 ────┼─┤      │  │
         │          │ GPIO 34 ────┤ │      │  │
         │          │ GPIO 35 ────┤ │      │  │
         │          │               │      │  │
         │          │ GND ──────────┤──┐   │  │
         │          │               │  │   │  │
         └─────────►│ VCC 5V ───────┤──┼───┘  │
                    │               │  │      │
                    └─────────────────┼──────┘
                                      │
        ┌─────────────────────────────┼─────────────────┐
        │                             │                 │
        ▼                             ▼                 ▼
    ┌────────────┐            ┌──────────────┐    ┌──────────┐
    │   Motor    │            │   MPU6050    │    │ Encoder  │
    │   Driver   │            │    (IMU)     │    │ Rotary   │
    └────────────┘            └──────────────┘    └──────────┘
         │                           │                 │
    Motor Left                   3.3V VCC          5V VCC
    Motor Right                 I2C Data           Quad A,B
                               (Acc + Gyro)       (Speed)
```

---

## ✅ Danh Sách Kiểm Tra Kết Nối

- [ ] Motor Left (IN3, IN4) kết nối đến GPIO 14, 27
- [ ] Motor Right (IN1, IN2) kết nối đến GPIO 12, 13
- [ ] Encoder Left (A, B) kết nối đến GPIO 32, 33
- [ ] Encoder Right (A, B) kết nối đến GPIO 35, 34
- [ ] MPU6050 SDA kết nối đến GPIO 21
- [ ] MPU6050 SCL kết nối đến GPIO 22
- [ ] Tất cả GND được kết nối chung
- [ ] VCC 5V được cấp đúng cho Motor Driver và Encoder
- [ ] VCC 3.3V được cấp đúng cho MPU6050

---

## 📚 Tham Khảo Tài Liệu

- **ESP32 Pinout:** https://github.com/AzadRohDas/esp32_pinout
- **MPU6050 Datasheet:** https://invensense.tdk.com/wp-content/uploads/2015/02/MPU-6000-Datasheet1.pdf
- **PCNT (Pulse Counter):** https://docs.espressif.com/projects/esp-idf/en/stable/esp32/api-reference/peripherals/pcnt.html

---

**Phiên bản:** 1.0  
**Cập nhật lần cuối:** 2026-04-27  
**Robot:** Self Balancing Robot V3
