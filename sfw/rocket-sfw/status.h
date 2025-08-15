#ifndef STRUCT_DEFINITIONS_H
#define STRUCT_DEFINITIONS_H

#include <Arduino.h>

typedef struct {
  bool aht = true;
  bool bmp = true;
  bool bno = true;
  bool ccs = true;
  bool gps = true;
  bool ina = true;
  bool openlog = true;
  bool rtc = true;
  bool xbee = true;
} components_status_struct;

extern components_status_struct components_status;

bool get_component_status(String component_name);
void set_component_status(String component_name, bool status);

#endif
