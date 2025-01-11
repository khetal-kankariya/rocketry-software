#include "telemetry.h"

#include <Arduino.h>
#include <Wire.h>

sensor_status_struct sensor_status;

int state_num;

int millis_time;
int millis_time_init;

int rtc_time;

float bmp_altitude, bmp_pressure, aht_temperature, ina_voltage, l89_time,
    l89_altitude, l89_latitude, l89_longitude, l89_satellites,
    bno_acceleration_x, bno_acceleration_y, bno_acceleration_z, gyro_spin_rate,
    mics_gas_concentration;

void setup() {
  setup_sensors();
  millis_time_init = millis();
  Serial.begin(115200);
}

void loop() {

  if (Serial.available()) {
    check_sensor_status();

    make_telemetry_data();
    save_prev_data(telemetry_data);

    Serial.println(telemetry_string);

    delay(1000);
  }
}

void setup1() { Serial.begin(115200); }

void loop1() {
  if (is_fresh_start()) {
    // BOOT to SETUP
    increment_software_state();

    String command;
    command = Serial.read();

    if (command == "telestart") {
      Serial.read();
    }
    if (command == "telestop") {
      // Serial.;
    }
    if (command == "calibrate") {
      int i = 5;
      while (i--) {
        if (calibrate_sensors()) {

          // SETUP to LAUNCH
          increment_software_state();
          break;
        };
      }
    }
  } else {
    // TODO how to check latest software state
    int latest_software_state;

    set_software_state(latest_software_state);
  }

  if (software_state_num == 2) {
    if (bno_acceleration_y > 0 && bmp_altitude > 0 && l89_altitude > 0) {
      increment_software_state();
    }
  } else if (software_state_num == 3) {
    if (bmp_altitude >= 1000 || velocity <= 0) {
    }
  }
}
