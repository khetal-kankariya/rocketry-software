#include "calculated_data.h"

float velocity;

float get_velocity() {
  velocity = telemetry_data.altitude - prev_telemetry_data.altitude;
  return (telemetry_data.altitude - prev_telemetry_data.altitude);
}
