#include "softwarestates.h"

String software_state_string = "BOOT";
int software_state_num = 0;

bool is_calibrated = false;


void calibrate() {
  int i = 0;
  while (i++ < 50) {
    if (calibrate_sensors()) {
      start_telemetry();
      is_calibrated = true;
      return;
    };
  }
  is_calibrated = false;
}


void software_state_manager() {
  // String command = read_from_xbee();

  // if (command == "telestart") {
  //   start_telemetry();
  // } else if (command == "telestop") {
  //   stop_telemetry();
  // }

  // if (is_fresh_start() == false) {
  //   int latest_software_state_num = get_last_state_num();
  //   Serial.println(software_state_num);

  //   set_software_state(latest_software_state_num);
  // }


  switch (software_state_num) {
    // BOOT to SETUP
    case 0:
      increment_software_state();
      break;
    // SETUP to LAUNCH
    case 1:
      // if (command == "calibrate") {
      //   stop_telemetry();
      //   calibrate();
      // }
      if (is_calibrated) increment_software_state();
      break;
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

String state_numToString(int num) {
  switch (num) {
    case 0:
      return "BOOT";
    case 1:
      return "SETUP";
    case 2:
      return "LAUNCH";
    case 3:
      return "ASCENT";
    case 4:
      return "PAYLOAD_SEP";
    case 5:
      return "DESCENT";
    case 6:
      return "IMPACT";
    default:
      return "invalid state";
  }
}

int state_stringToNum(String str) {
  if (str == "BOOT")
    return 0;
  else if (str == "SETUP")
    return 1;
  else if (str == "LAUNCH")
    return 2;
  else if (str == "ASCENT")
    return 3;
  else if (str == "PAYLOAD_SEP")
    return 4;
  else if (str == "DESCENT")
    return 5;
  else if (str == "IMPACT")
    return 6;
  else
    return 99;
}

void increment_software_state() {
  software_state_num += 1;
  software_state_string = state_numToString(software_state_num);
}

void set_software_state(int new_software_state) {
  software_state_num = new_software_state;
  software_state_string = state_numToString(software_state_num);
}

String get_software_state_string() {
  return software_state_string;
}
