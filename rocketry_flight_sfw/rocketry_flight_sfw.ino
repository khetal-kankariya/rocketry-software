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

bool send_telemetry = false;

void setup() {
  setup_sensors();
  Serial.begin(115200);
}

void loop() {

  if (send_telemetry) {
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
    if (software_state_num == 0) {
      increment_software_state();
      Serial.println(software_state_num);
    }

    String command = Serial.readString();

    if (command == "telestart") {

      send_telemetry = true;
    }
    if (command == "telestop") {
      send_telemetry = false;
    }
    if (command == "calibrate") {
      if (calibrate_sensors()) {
        // SETUP to LAUNCH
        if (software_state_num == 1) {
          increment_software_state();
        }
      }
    }
  } else {
    // TODO how to check latest software state
    int latest_software_state_num = get_last_state_num();

    set_software_state(latest_software_state_num);
  }

  switch (software_state_num) {
  case 2:
    if (velocity > 0 && bmp_altitude > 0 && l89_altitude > 0) {
      increment_software_state();
    }
    break;
  case 3:
    if (bmp_altitude >= 1000 || velocity <= 0) {
      increment_software_state();
    }
    break;
  case 4:
    trigger_burnwire();
    increment_software_state();
    break;
  case 5:
    if (velocity == 0 || bmp_altitude == 0) {
      increment_software_state();
    }
  case 6:
    sound_buzzer();
    break;
  default:
    break;
  }

  delay(100);
}
