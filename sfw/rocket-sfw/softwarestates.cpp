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

String get_software_state_string() { return software_state_string; }
