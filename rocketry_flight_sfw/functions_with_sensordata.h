// #ifndef FUNCTIONS_WITH_SENSORDATA_H
// #define FUNCTIONS_WITH_SENSORDATA_H

// #include "calculated_data.h"
// #include "previous_data.h"
// #include "telemetry.h"
// #include <Arduino.h>

// #define SEALEVELPRESSURE_HPA (1013.25)
// #define MICS_I2C_ADDRESS MICS_ADDRESS_0 // 0x75

// #include <Adafruit_AHTX0.h>
// #include <Adafruit_BMP3XX.h>
// #include <Adafruit_BNO055.h>
// #include <Adafruit_Sensor.h>
// #include <DFRobot_MICS.h>
// #include <INA226.h>
// #include <RTClib.h>
// #include <SoftwareSerial.h>
// #include <TinyGPSPlus.h>
// #include <Wire.h>
// #include <utility/imumaths.h>

// extern Adafruit_AHTX0 aht;
// extern Adafruit_BMP3XX bmp;
// extern Adafruit_BNO055 bno;
// extern TinyGPSPlus gps;
// extern RTC_DS3231 rtc;
// extern DFRobot_MICS_I2C mics;
// extern INA226 INA;

// void setup_sensors();

// extern float reference_pressure, reference_altitude;
// extern float reference_orientation_x, reference_orientation_y,
//     reference_orientation_z;

// void check_sensor_status();
// bool set_reference_alt_pressure();
// bool set_reference_orientation();

// String read_time();
// int read_packet_count();
// float read_altitude(float pressure);
// float read_pressure();
// float read_temperature();
// float read_voltage();
// float read_velocity();
// int read_gnss_time();
// float read_gnss_altitude();
// float read_gnss_latitude();
// float read_gnss_longitude();
// float read_gnss_satellites();
// float read_orientation_x();
// float read_orientation_y();
// float read_orientation_z();
// float read_acceleration_x();
// float read_acceleration_y();
// float read_acceleration_z();
// float read_gyro_spin_rate();
// float read_gas_concentration();

// bool is_fresh_start();
// bool calibrate_sensors();

// #endif
