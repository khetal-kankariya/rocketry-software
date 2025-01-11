#ifndef STRUCT_DEFINITIONS_H
#define STRUCT_DEFINITIONS_H

#include <Arduino.h>

typedef struct {
  int ds3231 = 1;
  int bmp390 = 1;
  int aht21 = 1;
  int mics = 1;
  int l89 = 1;
  int bno055 = 1;
  int ina226 = 1;
} sensor_status_struct;

typedef struct {
  String teamID = "2024ASI-ROCKETRY-026";
  String time;
  int packet_count;
  float altitude;
  float pressure;
  float temperature;
  float voltage;
  int gnss_time;
  float gnss_latitude;
  float gnss_longitude;
  float gnss_altitude;
  int gnss_satellites;
  float orientation_x;
  float orientation_y;
  float orientation_z;
  float acceleration_x;
  float acceleration_y;
  float acceleration_z;
  float gyro_spin_rate;
  String software_state;
  float gas_concentration;
  float velocity;
} telemetry_data_struct;

#endif
