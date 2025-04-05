#ifndef PREVIOUS_DATA_H
#define PREVIOUS_DATA_H

#include <Arduino.h>

#include "telemetry.h"

extern telemetry_data_struct prev_telemetry_data;

void save_prev_data(telemetry_data_struct prev_data);
#endif
