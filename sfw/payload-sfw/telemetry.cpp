#include "telemetry.h"

#include <Arduino.h>

telemetry_data_struct telemetry_data;
String telemetry_string;

void make_telemetry_data() {
  telemetry_data.teamID = "2024-ASI-ROCKETRY-026";
  telemetry_data.time = read_time();
  telemetry_data.packet_count = read_packet_count();
  telemetry_data.altitude = read_altitude();
  telemetry_data.pressure = read_pressure();
  telemetry_data.temperature = read_temperature();
  telemetry_data.orientation_x = read_orientation_x();
  telemetry_data.orientation_y = read_orientation_y();
  telemetry_data.orientation_z = read_orientation_z();
  telemetry_data.acceleration_x = read_acceleration_x();
  telemetry_data.acceleration_y = read_acceleration_y();
  telemetry_data.acceleration_z = read_acceleration_z();
  telemetry_data.gas_concentration = read_gas_concentration();
  telemetry_data.velocity = read_velocity();
}

void make_telemetry_data_string() {

  String accelerometer_data_string =
    String(telemetry_data.orientation_x) + "," + String(telemetry_data.orientation_y) + "," + String(telemetry_data.orientation_z) + "," + String(telemetry_data.acceleration_x) + "," + String(telemetry_data.acceleration_y) + "," + String(telemetry_data.acceleration_z);

  telemetry_string =
    String(telemetry_data.teamID) + "," + String(telemetry_data.time) + "," + String(telemetry_data.packet_count) + "," + String(telemetry_data.altitude) + "," + String(telemetry_data.pressure) + "," + String(telemetry_data.temperature) + "," + "," + accelerometer_data_string + "," + String(telemetry_data.gas_concentration);
}
