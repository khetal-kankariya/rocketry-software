#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include "calculated_data.h"
#include "previous_data.h"
#include "telemetry.h"
#include <Arduino.h>

#define SEALEVELPRESSURE_HPA (1013.25)
#define MICS_I2C_ADDRESS MICS_ADDRESS_0 // 0x75
#define burnwire_pin 3
#define buzzer_pin 5

#include <Adafruit_AHTX0.h>
#include <Adafruit_BMP3XX.h>
#include <Adafruit_BNO055.h>
#include <Adafruit_CCS811.h>
#include <Adafruit_Sensor.h>
#include <INA226.h>
#include <RTClib.h>
#include <SoftwareSerial.h>
#include <TinyGPS++.h>
#include <Wire.h>
#include <utility/imumaths.h>

extern Adafruit_AHTX0 aht;
extern Adafruit_BMP3XX bmp;
extern Adafruit_BNO055 bno;
extern TinyGPSPlus gps;
extern SoftwareSerial gps_serial;
extern RTC_DS3231 rtc;
extern INA226 INA;
extern SoftwareSerial openLog;

void setup_sensors();

extern float reference_pressure, reference_altitude;
extern float reference_orientation_x, reference_orientation_y,
    reference_orientation_z;

bool set_reference_alt_pressure();
bool set_reference_orientation();

String read_time();
String time_formatting(int raw_time);
int read_packet_count();
float read_altitude();
float read_pressure();
float read_temperature();
float read_voltage();
float read_velocity();
String read_gnss_time();
float read_gnss_altitude();
float read_gnss_latitude();
float read_gnss_longitude();
int read_gnss_satellites();
float read_orientation_x();
float read_orientation_y();
float read_orientation_z();
float read_acceleration_x();
float read_acceleration_y();
float read_acceleration_z();
String read_software_state();
float read_gas_concentration();

bool is_fresh_start();
bool calibrate_sensors();

void write_to_openlog(String telemetry_string);
void send_thru_xbee(String telemetry_string);
String get_last_openlog_log();
String get_last_state_string();
int get_last_state_num();

void trigger_burnwire();
void sound_buzzer();

bool sfw2_ready();
bool sfw3_ready();
bool sfw4_ready();
bool sfw5_ready();
bool sfw6_ready();

#endif
