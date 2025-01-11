#include "functions.h"
#include "calculated_data.h"
#include "previous_data.h"

Adafruit_AHTX0 aht;
Adafruit_BMP3XX bmp;
Adafruit_BNO055 bno = Adafruit_BNO055(55, 0x28, &Wire);
TinyGPSPlus gps;
RTC_DS3231 rtc;
DFRobot_MICS_I2C mics(&Wire, MICS_I2C_ADDRESS);
INA226 INA(0x40);

void setup_sensors() {
  Wire.setSDA(4);
  Wire.setSCL(5);
  Wire.begin();
  Wire.setClock(400000);

  bmp.setTemperatureOversampling(BMP3_OVERSAMPLING_8X);
  bmp.setPressureOversampling(BMP3_OVERSAMPLING_4X);
  bmp.setIIRFilterCoeff(BMP3_IIR_FILTER_COEFF_3);
  bmp.setOutputDataRate(BMP3_ODR_50_HZ);
}

float reference_pressure, reference_altitude;
float reference_orientation_x, reference_orientation_y,
reference_orientation_z; int packet_count = 0;

bool set_reference_alt_pressure() {
  float init_pressure = 0;
  for (int i = 0; i < 50; i++) {
    init_pressure += read_pressure();
  }
  init_pressure /= 50;
  reference_pressure = init_pressure;
  reference_altitude = read_altitude(init_pressure);

  return (0 == (read_altitude(read_pressure()) - reference_altitude) ? true
                                                                     :
                                                                     false);
}

bool set_reference_orientation() {
  float init_orientation_x = 0, init_orientation_y = 0, init_orientation_z =
  0;

  for (int i = 0; i < 50; i++) {
    init_orientation_x += read_orientation_x();
    init_orientation_y += read_orientation_y();
    init_orientation_z += read_orientation_z();
  }

  init_orientation_x /= 50;
  init_orientation_y /= 50;
  init_orientation_z /= 50;

  reference_orientation_x = init_orientation_x;
  reference_orientation_y = init_orientation_y;
  reference_orientation_z = init_orientation_z;

  return ((0 == (read_orientation_x() - reference_orientation_x) &&
           0 == (read_orientation_y() - reference_orientation_y) &&
           0 == (read_orientation_z() - reference_orientation_z))
              ? true
              : false);
};

void check_sensor_status() {
  bool rtc_working = true, bno_wokring = true, bmp_wokring = true,
       aht_wokring = true, gnss_working = true, ina_wokring = true;

  //! check sensor status

  // return
}

// RTC reading
String read_time() {

  unsigned long totalTime = millis() / 1000.0;
  DateTime now = rtc.now();

  String time_string = String(now.hour(), DEC) + ": " +
                       String(now.minute(), DEC) + ": " +
                       String(now.second(), DEC);

  return time_string;
};

// Packet Count
int read_packet_count() { return packet_count+=1; }

// BMP reading
float read_altitude(float pressure) { return 0; };
float read_pressure() { return 0; };

// AHT reading
float read_temperature() { return 0; };

// INA reading
float read_voltage() { return 0; };

// velocity reading
float read_velocity() {
  // return (telemetry_data.altitude - prev_telemetry_data.altitude);
  return get_velocity();
}

// GNSS reading
int read_gnss_time() { return 0; };
float read_gnss_altitude() { return 0; };
float read_gnss_latitude() { return 0; };
float read_gnss_longitude() { return 0; };
float read_gnss_satellites() { return 0; };

// BNO orientation reading
float read_orientation_x() { return 0; };
float read_orientation_y() { return 0; };
float read_orientation_z() { return 0; };

// BNO acceleration reading
float read_acceleration_x() { return 0; };
float read_acceleration_y() { return 0; };
float read_acceleration_z() { return 0; };

// Gyro Rate reading
//! not sure if we implement this
float read_gyro_spin_rate() { return 0; };

// MiCS reading
float read_gas_concentration() { return 0; };

bool is_fresh_start() {
  // TODO check previous logs and return true if fresh start

  return true;
}

bool calibrate_sensors() {
  return (set_reference_alt_pressure() && set_reference_orientation());
}
