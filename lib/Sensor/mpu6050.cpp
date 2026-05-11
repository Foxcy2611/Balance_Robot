#include <mpu6050.h>

static uint8_t check = 0;

void I2C_WriteReg(uint8_t addr, uint8_t regAddr, uint8_t data){
    Wire.beginTransmission(addr);
    Wire.write(regAddr);
    Wire.write(data);
    Wire.endTransmission();
}


void I2C_ReadReg(uint8_t addr, uint8_t regAddr, uint8_t *buff, uint8_t len){
    Wire.beginTransmission(addr);
    Wire.write(regAddr);
    Wire.endTransmission(false);

    Wire.requestFrom(addr, len);

    while(Wire.available() && len > 0){
        *buff++ = Wire.read();
        len--;
    }
}

void MPU6050_Init(void){
    Wire.begin(I2C_SDA, I2C_SCL);
    Wire.setClock(100000);

    I2C_ReadReg(MPU_ADDR, REG_WHO_AM_I, &check, 1);
    if(check == 0x68){
        I2C_WriteReg(MPU_ADDR, REG_PWR_MGMT_1, 0x00);
        I2C_WriteReg(MPU_ADDR, REG_SMPLRT_DIV, 0x07);
        I2C_WriteReg(MPU_ADDR, REG_CONFIG, 0x00);
		I2C_WriteReg(MPU_ADDR, REG_GYRO_CONFIG, 0x00);
		I2C_WriteReg(MPU_ADDR, REG_ACCEL_CONFIG, 0x00);
    } else {
        Serial.println("INIT MPU6050 Error !\r\n");
    }
}

void MPU6050_ReadAccel(int16_t *Ax, int16_t *Ay, int16_t *Az){
    uint8_t buf[6];
    I2C_ReadReg(MPU_ADDR, REG_ACCEL_XOUT_H, buf, 6);

    *Ax = (int16_t)(buf[0] << 8 | buf[1]);
    *Ay = (int16_t)(buf[2] << 8 | buf[3]);
    *Az = (int16_t)(buf[4] << 8 | buf[5]);
}

void MPU6050_ReadGyro(int16_t *Gx, int16_t *Gy, int16_t *Gz){
    uint8_t buf[6];
    I2C_ReadReg(MPU_ADDR, REG_GYRO_XOUT_H, buf, 6);

    *Gx = (int16_t)(buf[0] << 8 | buf[1]);
    *Gy = (int16_t)(buf[2] << 8 | buf[3]);
    *Gz = (int16_t)(buf[4] << 8 | buf[5]);
}