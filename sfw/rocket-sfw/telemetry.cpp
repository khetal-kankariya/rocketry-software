#include "telemetry.h"

telemetry_data_struct telemetry_data;
String telemetry_string;

void make_telemetry_data() {
  telemetry_data.teamID = "2024ASI-ROCKETRY-026";
  telemetry_data.time = read_time();
  telemetry_data.packet_count = read_packet_count();
  telemetry_data.pressure = read_pressure();
  telemetry_data.altitude = read_altitude();
  telemetry_data.temperature = read_temperature();
  telemetry_data.voltage = read_voltage();
  telemetry_data.gnss_time = read_gnss_time();
  telemetry_data.gnss_latitude = read_gnss_latitude();
  telemetry_data.gnss_altitude = read_gnss_altitude();
  telemetry_data.gnss_longitude = read_gnss_longitude();
  telemetry_data.gnss_satellites = read_gnss_satellites();
  telemetry_data.orientation_x = read_orientation_x();
  telemetry_data.orientation_y = read_orientation_y();
  telemetry_data.orientation_z = read_orientation_z();
  telemetry_data.acceleration_x = read_acceleration_x();
  telemetry_data.acceleration_y = read_acceleration_y();
  telemetry_data.acceleration_z = read_acceleration_z();
  telemetry_data.software_state = read_software_state();
  telemetry_data.gas_concentration = read_gas_concentration();
  telemetry_data.velocity = read_velocity();

  // telemetry_data.teamID = "2024ASI-ROCKETRY-026";
  // telemetry_data.time = "00:00:00";
  // telemetry_data.packet_count = 0;
  // telemetry_data.pressure = 0;
  // telemetry_data.altitude = 0;
  // telemetry_data.temperature = 0;
  // telemetry_data.voltage = 0;
  // telemetry_data.gnss_time = "00:00:00";
  // telemetry_data.gnss_latitude = 0;
  // telemetry_data.gnss_altitude = 0;
  // telemetry_data.gnss_longitude = 0;
  // telemetry_data.gnss_satellites = 0;
  // telemetry_data.orientation_x = 0;
  // telemetry_data.orientation_y = 0;
  // telemetry_data.orientation_z = 0;
  // telemetry_data.acceleration_x = 0;
  // telemetry_data.acceleration_y = 0;
  // telemetry_data.acceleration_z = 0;
  // telemetry_data.software_state = "BOOT";
  // telemetry_data.gas_concentration = 0;
  // telemetry_data.velocity = 0;

  make_telemetry_data_string();
}

void make_telemetry_data_string() {

  String gnss_data_string = String(telemetry_data.gnss_time) + "," + String(telemetry_data.gnss_latitude) + "," + String(telemetry_data.gnss_longitude) + "," + String(telemetry_data.gnss_altitude) + "," + String(telemetry_data.gnss_satellites);

  String accelerometer_data_string =
    String(telemetry_data.orientation_x) + "," + String(telemetry_data.orientation_y) + "," +

    String(telemetry_data.orientation_z) + "," + String(telemetry_data.acceleration_x) + "," + String(telemetry_data.acceleration_y) + "," + String(telemetry_data.acceleration_z);

  telemetry_string =
    String(telemetry_data.teamID) + "," + String(telemetry_data.time) + "," + String(telemetry_data.packet_count) + "," + String(telemetry_data.altitude) + "," + String(telemetry_data.pressure) + "," + String(telemetry_data.temperature) + "," + String(telemetry_data.voltage) + "," + gnss_data_string + "," + accelerometer_data_string + "," + (telemetry_data.software_state) + "," + String(telemetry_data.gas_concentration);
}
