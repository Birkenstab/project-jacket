#include "Wire.h" // This library allows you to communicate with I2C devices.

char tmp_str[7]; // temporary variable used in convert function
char* convert_int16_to_str(int16_t i) { // converts int16 to string. Moreover, resulting strings will have the same length in the debug monitor.
  sprintf(tmp_str, "%6d", i);
  return tmp_str;
}

struct SensorValue {
    int16_t accX, accY, accZ; // accelerometer
    int16_t gyroX, gyroY, gyroZ; // gyrocope
    int16_t temp; // temperature
};

SensorValue readGyroValues(int sensorNo); // Ja das braucht man, weil die tolle Arduino IDE die Funktionen alle am Anfang deklariert, wo SensorValue noch nicht definiert ist
void printGyroValue(const SensorValue& value);

void switchToBus(int sensorNo) {
    if (sensorNo <= 1) {
        Wire.begin(4, 5);
    } else {
        Wire.begin(2, 0);
    }
}

int getSensorAddr(int sensorNo) {
    if (sensorNo % 2 == 0) {
        return 0x68;
    } else {
        return 0x69;
    }
}

void initGyro(int sensorNo) {
    Serial.print("Initializing Gyro ");
    Serial.print(sensorNo);
    Serial.println(" ...");

    switchToBus(sensorNo);
    
    Wire.beginTransmission(getSensorAddr(sensorNo)); // Begins a transmission to the I2C slave (GY-521 board)
    Wire.write(0x6B); // PWR_MGMT_1 register
    Wire.write(0); // set to zero (wakes up the MPU-6050)
    Wire.endTransmission(true);
}

SensorValue readGyroValues(int sensorNo) {
    switchToBus(sensorNo);
    uint8_t addr = getSensorAddr(sensorNo);

    SensorValue sensorValue;

    Wire.beginTransmission(addr);
    Wire.write(0x3B); // starting with register 0x3B (ACCEL_XOUT_H) [MPU-6000 and MPU-6050 Register Map and Descriptions Revision 4.2, p.40]
    Wire.endTransmission(false); // the parameter indicates that the Arduino will send a restart. As a result, the connection is kept active.
    Wire.requestFrom(addr, static_cast<uint8_t>(7 * 2), static_cast<uint8_t>(true)); // request a total of 7*2=14 registers

    // "Wire.read()<<8 | Wire.read();" means two registers are read and stored in the same variable
    sensorValue.accX = Wire.read()<<8 | Wire.read(); // reading registers: 0x3B (ACCEL_XOUT_H) and 0x3C (ACCEL_XOUT_L)
    sensorValue.accY = Wire.read()<<8 | Wire.read(); // reading registers: 0x3D (ACCEL_YOUT_H) and 0x3E (ACCEL_YOUT_L)
    sensorValue.accZ = Wire.read()<<8 | Wire.read(); // reading registers: 0x3F (ACCEL_ZOUT_H) and 0x40 (ACCEL_ZOUT_L)
    sensorValue.temp = Wire.read()<<8 | Wire.read(); // reading registers: 0x41 (TEMP_OUT_H) and 0x42 (TEMP_OUT_L)
    sensorValue.gyroX = Wire.read()<<8 | Wire.read(); // reading registers: 0x43 (GYRO_XOUT_H) and 0x44 (GYRO_XOUT_L)
    sensorValue.gyroY = Wire.read()<<8 | Wire.read(); // reading registers: 0x45 (GYRO_YOUT_H) and 0x46 (GYRO_YOUT_L)
    sensorValue.gyroZ = Wire.read()<<8 | Wire.read(); // reading registers: 0x47 (GYRO_ZOUT_H) and 0x48 (GYRO_ZOUT_L)

    return sensorValue;
}

void printGyroValue(const SensorValue& value) {
    Serial.print(convert_int16_to_str(value.accX));
    Serial.print("\t"); 
    Serial.print(convert_int16_to_str(value.accY));
    Serial.print("\t"); 
    Serial.print(convert_int16_to_str(value.accZ));
    Serial.print("\t"); 

    Serial.print(convert_int16_to_str(value.gyroX));
    Serial.print("\t"); 
    Serial.print(convert_int16_to_str(value.gyroY));
    Serial.print("\t"); 
    Serial.print(convert_int16_to_str(value.gyroZ));
    Serial.print("\t");
}

void setup() {
  Serial.begin(115200);
  Serial.println("Guten Morgen");

  Wire.setClock(400000); // 400 kHz Clock

  initGyro(0);
  initGyro(1);
  initGyro(2);
}

void loop() {
  SensorValue value0 = readGyroValues(0);
  SensorValue value1 = readGyroValues(1);
  SensorValue value2 = readGyroValues(2);
  
  printGyroValue(value0);
  printGyroValue(value1);
  printGyroValue(value2);
  Serial.println();
  
  delay(10);
}
