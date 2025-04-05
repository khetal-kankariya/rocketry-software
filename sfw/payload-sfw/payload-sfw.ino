#include "telemetry.h"

#include <Arduino.h>
#include <Wire.h>

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

    String command = read_from_xbee();

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
