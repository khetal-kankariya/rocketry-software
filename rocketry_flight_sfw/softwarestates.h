#ifndef SOFTWARESTATES_H
#define SOFTWARESTATES_H

#include <Arduino.h>

extern String software_state_string;
extern int software_state_num;

String state_numToString(int num);
int state_stringToNum(String str);
String get_software_state_string();
void increment_software_state();
void set_software_state(int new_software_state);

#endif
