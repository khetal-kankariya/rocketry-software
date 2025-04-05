#ifndef DATA_MANAGER_H
#define DATA_MANAGER_H

#include <Arduino.h>

extern int rtc_time;

extern float bmp_altitude;
extern float bmp_pressure;

extern float aht_temperature;

extern float ina_voltage;

extern float l89_time;
extern float l89_altitude;
extern float l89_latitude;
extern float l89_longitude;
extern float l89_satellites;

extern float bno_acceleration_x;
extern float bno_acceleration_y;
extern float bno_acceleration_z;

extern float gyro_spin_rate;

extern float mics_gas_concentration;
#endif
