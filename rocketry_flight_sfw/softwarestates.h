#ifndef SOFTWARESTATES_H
#define SOFTWARESTATES_H

#include <Arduino.h>

extern String software_state_string;
extern int software_state_num;

String state_numToString(int num);
String read_software_state();
String get_software_state();
void increment_software_state();
void set_software_state(int new_software_state);

#endif
