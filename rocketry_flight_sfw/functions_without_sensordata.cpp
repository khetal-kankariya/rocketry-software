#include "functions_without_sensordata.h"
#include "calculated_data.h"
#include "previous_data.h"

float reference_pressure = 0, reference_altitude = 0;
float reference_orientation_x = 0, reference_orientation_y = 0,
      reference_orientation_z = 0;
int packet_count = 1;

bool set_reference_alt_pressure() {
  float init_pressure = 0;
  for (int i = 0; i < 50; i++) {
    init_pressure += read_pressure();
  }
  init_pressure /= 50;
  reference_pressure = init_pressure;
  reference_altitude = read_altitude(init_pressure);

  return (0 == (read_altitude(read_pressure()) - reference_altitude) ? true
                                                                     : false);
}
void setup_sensors() {}

bool set_reference_orientation() {
  float init_orientation_x = 0, init_orientation_y = 0, init_orientation_z = 0;

  for (int i = 0; i < 50; i++) {
    init_orientation_x += read_orientation_x();
    init_orientation_y += read_orientation_y();
    init_orientation_z += read_orientation_z();
  }

  init_orientation_x /= 50;
  init_orientation_y /= 50;
  init_orientation_z /= 50;

  reference_orientation_x = init_orientation_x;
  reference_orientation_y = init_orientation_y;
  reference_orientation_z = init_orientation_z;

  return ((0 == (read_orientation_x() - reference_orientation_x) &&
           0 == (read_orientation_y() - reference_orientation_y) &&
           0 == (read_orientation_z() - reference_orientation_z))
              ? true
              : false);
};

void check_sensor_status() {
  bool rtc_working = true, bno_wokring = true, bmp_wokring = true,
       aht_wokring = true, gnss_working = true, ina_wokring = true;

  //! check sensor status

  // return
}

// RTC reading
String read_time() {
  String time_string = "00:00:00";
  return time_string;
};

// Packet Count
int read_packet_count() { return packet_count++; }

// BMP reading
float read_altitude(float pressure) { return float(random(0, 900)) / 10.0; };
float read_pressure() { return float(random(0, 900)) / 10.0; };

// AHT reading
float read_temperature() { return float(random(0, 900)) / 10.0; };

// INA reading
float read_voltage() { return float(random(0, 900)) / 10.0; };

// velocity reading
float read_velocity() {
  // return (telemetry_data.altitude - prev_telemetry_data.altitude);
  return get_velocity();
}

// GNSS reading
int read_gnss_time() { return float(random(0, 100)); };
float read_gnss_altitude() { return float(random(0, 900)) / 10.0; };
float read_gnss_latitude() { return float(random(0, 900)) / 10.0; };
float read_gnss_longitude() { return float(random(0, 900)) / 10.0; };
int read_gnss_satellites() { return int(random(0, 100)); };

// BNO orientation reading
float read_orientation_x() { return float(random(0, 900)) / 10.0; };
float read_orientation_y() { return float(random(0, 900)) / 10.0; };
float read_orientation_z() { return float(random(0, 900)) / 10.0; };

// BNO acceleration reading
float read_acceleration_x() { return (random(0, 900)) / 10.0; };
float read_acceleration_y() { return (random(0, 900)) / 10.0; };
float read_acceleration_z() { return (random(0, 900)) / 10.0; };

// Gyro Rate reading
//! not sure if we implement this
float read_gyro_spin_rate() { return float(random(0, 900)) / 10.0; };

// MiCS reading
float read_gas_concentration() { return float(random(0, 900)) / 10.0; };

bool is_fresh_start() {
  // TODO check previous logs and return true if fresh start

  return true;
}

bool calibrate_sensors() {
  return (set_reference_alt_pressure() && set_reference_orientation());
}
