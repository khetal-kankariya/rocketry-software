#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include "calculated_data.h"
#include "previous_data.h"
#include "telemetry.h"
#include <Arduino.h>

#define SEALEVELPRESSURE_HPA (1013.25)
#define burnwire_pin 15
#define buzzer_pin 5

#include <Adafruit_AHTX0.h>
#include <Adafruit_BMP3XX.h>
#include <Adafruit_BNO055.h>
#include <Adafruit_CCS811.h>
#include <Adafruit_Sensor.h>
#include <RTClib.h>
#include <SoftwareSerial.h>
#include <Wire.h>
#include <utility/imumaths.h>

extern Adafruit_AHTX0 aht;
extern Adafruit_BMP3XX bmp;
extern Adafruit_BNO055 bno;
extern RTC_DS3231 rtc;
extern SoftwareSerial openLog;

void setup_sensors();

extern float reference_pressure, reference_altitude;
extern float reference_orientation_x, reference_orientation_y,
    reference_orientation_z;

bool set_reference_alt_pressure();
bool set_reference_orientation();

String read_time();
int read_packet_count();
float read_altitude();
float read_pressure();
float read_temperature();
float read_orientation_x();
float read_orientation_y();
float read_orientation_z();
float read_acceleration_x();
float read_acceleration_y();
float read_acceleration_z();
float read_gas_concentration();

void write_to_openlog(String telemetry_string);
void send_thru_xbee(String telemetry_string);
String read_from_xbee();

void sound_buzzer();

#endif
