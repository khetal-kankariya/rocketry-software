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

bool send_telemetry_flag = false;

void setup() {
  Serial.begin(115200);
  setup_sensors();
}

void loop() {
  fsw();

  if (send_telemetry_flag) {

    make_telemetry_data();
    save_prev_data(telemetry_data);

    write_to_openlog(telemetry_string);
    send_thru_xbee(telemetry_string);

    Serial.println(telemetry_string);

    delay(1000);
  }
}

void fsw() {
  if (is_fresh_start()) {

    // BOOT to SETUP
    if (software_state_num == 0) {
      increment_software_state();
    }

    String command = Serial.readString();
    command.trim();

    if (command == "telestart") {

      send_telemetry_flag = true;
    }
    if (command == "telestop") {
      send_telemetry_flag = false;
    }
    if (command == "calibrate") {
      while (!calibrate_sensors()) {
        send_telemetry_flag = false;
      }
      send_telemetry_flag = true;
      // SETUP to LAUNCH
      if (software_state_num == 1) {
        increment_software_state();
      }
    }
  } else {
    int latest_software_state_num = get_last_state_num();

    set_software_state(latest_software_state_num);
  }

  switch (software_state_num) {
  case 2:
    // LAUNCH to ASCENT
    if (sfw3_ready()) {
      increment_software_state();
    }
    break;
  case 3:
    // ASCENT to PAYLOAD_SEP
    if (sfw4_ready()) {
      increment_software_state();
    }
    break;
  case 4:
    trigger_burnwire();
    // PAYLOAD_SEP to DESCENT
    increment_software_state();
    break;
  case 5:
    // DESCENT to IMPACT
    if (sfw6_ready()) {
      increment_software_state();
    }
    break;
  case 6:
    sound_buzzer();
    break;
  default:
    break;
  }
}
