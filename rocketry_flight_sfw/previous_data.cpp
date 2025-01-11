#include "previous_data.h"

telemetry_data_struct prev_telemetry_data;

void save_prev_data(telemetry_data_struct prev_data) {
  prev_telemetry_data = prev_data;
}
