#ifndef TELEMETRY_H
#define TELEMETRY_H

#include <Arduino.h>

#include "header.h"
// #include "softwarestates.h"
// #include "structs.h"

extern telemetry_data_struct telemetry_data;
extern String telemetry_string;

void make_telemetry_data();
void make_telemetry_data_string();

#endif
