#include "softwarestates.h"

String software_state_string = "BOOT";
int software_state_num = 0;

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

String read_software_state() { return get_software_state(); };

void increment_software_state() {
  software_state_num += 1;
  state_numToString(software_state_num);
}

void set_software_state(int new_software_state) {
  software_state_num = new_software_state;
  state_numToString(software_state_num);
}

String get_software_state() { return "BOOT"; }
