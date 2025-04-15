#include "telemetry.h"

void setup() {
  Serial.begin(115200);
  setup_sensors();
}

void loop() {
  software_state_manager();

  // if (telemetry_state() == true) {

    make_telemetry_data();
    save_prev_data(telemetry_data);

    write_to_openlog(telemetry_string);
    // // send_thru_xbee(telemetry_string);

    Serial.println(telemetry_string);

    delay(1000);
  // }
}
