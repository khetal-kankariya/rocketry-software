#include "header.h"

void setup() {
  Serial.begin(115200);
  setup_sensors();
}

void loop() {
  software_state_manager();
  
  if (telemetry_state()) {
    make_telemetry_data();
    save_prev_data(telemetry_data);

    write_to_openlog(telemetry_string);
    // write_to_eeprom(telemetry_data);
    // send_thru_xbee(telemetry_string);
    Serial.println(telemetry_string);

    // LoRa Sending
    // LoRa.beginPacket();
    // LoRa.print(telemetry_string);
    // LoRa.endPacket();
  }
  delay(1000);
}
