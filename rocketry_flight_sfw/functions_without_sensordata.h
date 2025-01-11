#ifndef FUNCTIONS_WITHOUT_SENSORDATA_H
#define FUNCTIONS_WITHOUT_SENSORDATA_H

#include "telemetry.h"
#include <Arduino.h>

void setup_sensors();

void check_sensor_status();
bool set_reference_alt_pressure();
bool set_reference_orientation();

String read_time();
int read_packet_count();
float read_altitude(float pressure);
float read_pressure();
float read_temperature();
float read_voltage();
float read_velocity();
int read_gnss_time();
float read_gnss_altitude();
float read_gnss_latitude();
float read_gnss_longitude();
int read_gnss_satellites();
float read_orientation_x();
float read_orientation_y();
float read_orientation_z();
float read_acceleration_x();
float read_acceleration_y();
float read_acceleration_z();
float read_gyro_spin_rate();
float read_gas_concentration();

bool is_fresh_start();
bool calibrate_sensors();

#endif
