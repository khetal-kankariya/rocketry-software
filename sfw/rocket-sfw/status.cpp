#include "status.h"

components_status_struct components_status;

bool get_component_status(String component_name) {
  if (component_name == "aht")
    return components_status.aht;
  else if (component_name == "bmp")
    return components_status.bmp;
  else if (component_name == "bno")
    return components_status.bno;
  else if (component_name == "ccs")
    return components_status.ccs;
  else if (component_name == "gps")
    return components_status.gps;
  else if (component_name == "ina")
    return components_status.ina;
  else if (component_name == "openlog")
    return components_status.openlog;
  else if (component_name == "rtc")
    return components_status.rtc;
  else if (component_name == "xbee")
    return components_status.xbee;
  else
    return false;
}

void set_component_status(String component_name, bool status) {
  if (component_name == "aht")
    components_status.aht = status;
  else if (component_name == "bmp")
    components_status.bmp = status;
  else if (component_name == "bno")
    components_status.bno = status;
  else if (component_name == "ccs")
    components_status.ccs = status;
  else if (component_name == "gps")
    components_status.gps = status;
  else if (component_name == "ina")
    components_status.ina = status;
  else if (component_name == "openlog")
    components_status.openlog = status;
  else if (component_name == "rtc")
    components_status.rtc = status;
  else if (component_name == "xbee")
    components_status.xbee = status;
}
