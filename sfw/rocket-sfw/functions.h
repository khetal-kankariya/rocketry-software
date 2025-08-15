
#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include "status.h"
#include "header.h"

#define SEALEVELPRESSURE_HPA (1013.25)
#define burnwire_pin 15
#define buzzer_pin 5

// LoRa stuff
#define DIO0_PIN 2
#define RST_PIN  9
#define SS_PIN   13
#define FREQUENCY 433E6
#define SYNC_WORD 0x12
#define TX_POWER  17
#define BANDWIDTH 125E3
#define SPREADING_FACTOR 7
#define CODING_RATE 5
#include <LoRa.h>

#include <Adafruit_AHTX0.h>
#include <Adafruit_BMP3XX.h>
#include <Adafruit_BNO055.h>
#include <Adafruit_CCS811.h>
#include <Adafruit_Sensor.h>
#include <INA226.h>
#include <RTClib.h>
#include <SoftwareSerial.h>
#include <TinyGPSPlus.h>
#include <Wire.h>
#include <utility/imumaths.h>
#include <EEPROM.h>

extern Adafruit_AHTX0 aht;
extern Adafruit_BMP3XX bmp;
extern Adafruit_BNO055 bno;
extern TinyGPSPlus gps;
extern SoftwareSerial ss;
extern RTC_DS3231 rtc;
extern Adafruit_CCS811 ccs;
extern INA226 INA;
extern SoftwareSerial openLog;

void setup_sensors();

extern float reference_pressure, reference_altitude;
extern float reference_orientation_x, reference_orientation_y,
  reference_orientation_z;
extern String last_log;

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

void write_to_eeprom(telemetry_data_struct telemetry_data);
int read_from_eeprom();
void write_to_openlog(String telemetry_string);
void send_thru_xbee(String telemetry_string);
String read_from_xbee();
String get_last_openlog_log();

void trigger_burnwire();
void sound_buzzer();

bool sfw2_ready();
bool sfw3_ready();
bool sfw4_ready();
bool sfw6_ready();

void start_telemetry();
void stop_telemetry();
bool telemetry_state();

#endif
