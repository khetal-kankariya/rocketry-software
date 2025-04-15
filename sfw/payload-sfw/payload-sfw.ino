#include "telemetry.h"

void setup() {
  Serial.begin(115200);
  setup_sensors();
}

void loop() {
  String receivedString = read_from_xbee();

  if (receivedString == "telestart") {
    send_telemetry_flag = true
  } else if (receivedString == "telestop") {
    send_telemetry_flag = false
  }

  if (send_telemetry_flag == true) {

    make_telemetry_data();
    make_telemetry_data_string();
    save_prev_data(telemetry_data);

    write_to_openlog(telemetry_string);
    send_thru_xbee(telemetry_string);

    Serial.println(telemetry_string);

    delay(1000);
  }
}
