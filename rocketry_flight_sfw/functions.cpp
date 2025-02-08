#include "functions.h"
#include "SoftwareSerial.h"
#include "calculated_data.h"
#include "previous_data.h"

#define SEALEVELPRESSURE_HPA (1013.25)

bool bmpstatus = true;

Adafruit_AHTX0 aht;
Adafruit_BMP3XX bmp;
Adafruit_BNO055 bno = Adafruit_BNO055(55, 0x28, &Wire);
TinyGPSPlus gps;
SoftwareSerial gps_serial(6, 7);
RTC_DS3231 rtc;
Adafruit_CCS811 ccs;
INA226 INA(0x40);
SoftwareSerial openLog(2, 3);

void setup_sensors() {
  // Wire.setSDA(4);
  // Wire.setSCL(5);
  // Wire.begin();
  // Wire.setClock(4000000);

  bmp.setTemperatureOversampling(BMP3_OVERSAMPLING_8X);
  bmp.setPressureOversampling(BMP3_OVERSAMPLING_4X);
  bmp.setIIRFilterCoeff(BMP3_IIR_FILTER_COEFF_3);
  bmp.setOutputDataRate(BMP3_ODR_50_HZ);

  // pinMode(burnwire_pin, OUTPUT);
  // pinMode(buzzer_pin, OUTPUT);

  aht.begin();
  bmp.begin_I2C();
  bno.begin();
  gps_serial.begin(115200);
  rtc.begin();
  ccs.begin();
  INA.begin();
  openLog.begin(115200);

  // set_reference_alt_pressure();
  // set_reference_orientation();

  rtc.adjust(DateTime(2025, 1, 1, 0, 0, 0));
}

float reference_pressure = SEALEVELPRESSURE_HPA, reference_altitude = 0;
float reference_orientation_x = 0, reference_orientation_y = 0,
      reference_orientation_z = 0;
sensors_event_t humidity, temp, orientationData, accelerationData;
int packet_count = 0;

bool set_reference_alt_pressure() {
  reference_pressure = 0;
  for (int i = 0; i < 5; i++) {
    float readalt = bmp.readAltitude(SEALEVELPRESSURE_HPA);
    while (readalt > 100) {
      readalt = bmp.readAltitude(SEALEVELPRESSURE_HPA);
    }
    reference_altitude += readalt;

    float readpressure = bmp.pressure / 100.0;
    while (readpressure < 100) {
      readpressure = bmp.pressure;
    }
    reference_pressure += readpressure;
  }

  reference_pressure /= 5;
  reference_altitude /= 5;

  return (true);
}

bool set_reference_orientation() {
  float init_orientation_x = 0, init_orientation_y = 0, init_orientation_z = 0;
  int i = 5;

  while (!((abs(read_orientation_x() - reference_orientation_x) < 1) &&
           (abs(read_orientation_y() - reference_orientation_y) < 1) &&
           (abs(read_orientation_z() - reference_orientation_z) < 1)) &&
         i) {

    for (int i = 0; i < 5; i++) {
      init_orientation_x += read_orientation_x();
      init_orientation_y += read_orientation_y();
      init_orientation_z += read_orientation_z();
    }

    init_orientation_x /= 5;
    init_orientation_y /= 5;
    init_orientation_z /= 5;

    reference_orientation_x = init_orientation_x;
    reference_orientation_y = init_orientation_y;
    reference_orientation_z = init_orientation_z;
    i--;
  }
  return (true);
};

void check_sensor_status() {
  bool rtc_working = true, bno_wokring = true, bmp_wokring = true,
       aht_wokring = true, gnss_working = true, ina_wokring = true;

  //! check sensor status

  // return
}

// RTC reading
String read_time() {
  DateTime now = rtc.now();

  String time_string = time_formatting(now.hour()) + ":" +
                       time_formatting(now.minute()) + ":" +
                       time_formatting(now.second());

  // String time_string = String("00") + ": " + String("00") + ": " + String("00");

  return time_string;
};

String time_formatting(int raw_time) {
  if (raw_time < 10) {
    return ("0" + String(raw_time));
  } else {
    return String(raw_time, DEC);
  }
}

// Packet Count
int read_packet_count() { return packet_count++; }

// BMP reading
float read_altitude() {
  return bmp.readAltitude(reference_pressure);
  // return 0;
};
float read_pressure() {
  return bmp.pressure / 100;
  // return 0;
};

//!
// AHT reading
float read_temperature() {
  aht.getEvent(&humidity, &temp);
  return temp.temperature;

  // return 0;
};

// INA reading
float read_voltage() {
  return INA.getBusVoltage();
  // return 0;
};

// velocity reading
float read_velocity() {
  return get_velocity();
}

// GNSS reading
String read_gnss_time() {
  gps.encode(gps_serial.read());
  String time_string= (time_formatting(gps.time.hour()) +
  ":"+time_formatting(gps.time.minute()) +
  ":"+time_formatting(gps.time.second()));
  // String time_string = String("00") + ": " + String("00") + ": " + String("00");
  return time_string;
};
float read_gnss_altitude() {
  return gps.altitude.meters();
  // return 0;
};
float read_gnss_latitude() {
  return gps.location.lat();
  // return 0;
};
float read_gnss_longitude() {
  return gps.location.lng();
  // return 0;
};
int read_gnss_satellites() {
  return gps.satellites.value()
  // return 0;
};

// BNO orientation reading
float read_orientation_x() {
  bno.getEvent(&orientationData, Adafruit_BNO055::VECTOR_EULER);
  orientationData.orientation.x -=
      ((orientationData.orientation.x > 180) ? 360 : 0);
  // return 0;
  return (orientationData.orientation.x - reference_orientation_x);
};
float read_orientation_y() {
  orientationData.orientation.y -=
      ((orientationData.orientation.y > 180) ? 360 : 0);
  // return 0;

  return (orientationData.orientation.y - reference_orientation_y);
};
float read_orientation_z() {
  orientationData.orientation.z -=
      ((orientationData.orientation.z > 180) ? 360 : 0);
  // return 0;
  return (orientationData.orientation.z - reference_orientation_z);
};

// BNO acceleration reading
float read_acceleration_x() {
  bno.getEvent(&accelerationData, Adafruit_BNO055::VECTOR_ACCELEROMETER);
  // return 0;
  return accelerationData.acceleration.x;
};

float read_acceleration_y() {
  // return 0;

  return accelerationData.acceleration.y;
};
float read_acceleration_z() {
  // return 0;
  return accelerationData.acceleration.z;
};

// Software State Reading
String read_software_state() { return get_software_state_string(); };

// MiCS reading (CO/CH4/C2H5OH/H2/NH3)
float read_gas_concentration() {
  return ccs.geteCO2();
  // return 0;
};

void trigger_burnwire() {
  digitalWrite(burnwire_pin, HIGH);
  delay(2500);
  digitalWrite(burnwire_pin, LOW);
}

void sound_buzzer() {

  digitalWrite(buzzer_pin, HIGH);
  delay(200);
  digitalWrite(buzzer_pin, LOW);
  delay(200);
  digitalWrite(buzzer_pin, HIGH);
  delay(200);
  digitalWrite(buzzer_pin, LOW);
  delay(500);
}

bool is_fresh_start() {
  // TODO check previous logs and return true if fresh start
  return (get_last_openlog_log().length() > 0);
}

bool calibrate_sensors() {
  return (set_reference_alt_pressure() && set_reference_orientation());
  // return (set_reference_alt_pressure());
  // return (set_reference_orientation());
}

void write_to_openlog(String telemetry_str) {
  openLog.println(telemetry_str);
}

String get_last_openlog_log() {
  String result = " ";
  String all_logs = openLog.readString();
  all_logs.trim();

  int length_all_logs = all_logs.length();

  if (length_all_logs > 0) {
    int startIndex = 0;
    int endIndex = 0;

    while (endIndex != -1) {
      endIndex = all_logs.indexOf('\n', startIndex);
      if (endIndex == -1) {
        result = all_logs.substring(startIndex);
      } else {
        result = all_logs.substring(startIndex, endIndex);
        startIndex = endIndex + 1;
      }
    }
  }

  return result;
}

String get_last_state_string() {
  String last_log = get_last_openlog_log();

  String last_state_string = state_numToString(software_state_num);
  int startIndex = 0;
  int endIndex = 0;

  while (endIndex != -1) {
    endIndex = last_log.indexOf(',', startIndex);
    if (endIndex == -1) {
      // last_state_num = last_log.substring(startIndex);
    } else {
      last_state_string = last_log.substring(startIndex, endIndex);
      startIndex = endIndex + 1;
    }
  }

  return last_state_string;
}

int get_last_state_num() {
  return state_stringToNum(prev_telemetry_data.software_state);
}
