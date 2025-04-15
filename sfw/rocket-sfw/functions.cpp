#include "functions.h"

#define SEALEVELPRESSURE_HPA (1013.25)

bool send_telemetry_flag = false;

Adafruit_AHTX0 aht;
Adafruit_BMP3XX bmp;
Adafruit_BNO055 bno = Adafruit_BNO055(55);
TinyGPSPlus gps;
SoftwareSerial gps_serial(9, 8);
// RTC_DS3231 rtc;
// Adafruit_CCS811 ccs;
INA226 INA(0x40);
SoftwareSerial openLog(1, 0);
SoftwareSerial xbee(13, 12);

float reference_pressure = SEALEVELPRESSURE_HPA;
float reference_orientation_x = 0, reference_orientation_y = 0,
      reference_orientation_z = 0;
sensors_event_t humidity, temp, orientationData, accelerationData;
int packet_count = 0;

bool calibrate_bmp();
bool calibrate_bno();

void setup_sensors() {
  Wire.setSDA(4);
  Wire.setSCL(5);

  Wire.begin();

  while (!Serial)
    ;

  bmp.setTemperatureOversampling(BMP3_OVERSAMPLING_8X);
  bmp.setPressureOversampling(BMP3_OVERSAMPLING_4X);
  bmp.setIIRFilterCoeff(BMP3_IIR_FILTER_COEFF_3);
  bmp.setOutputDataRate(BMP3_ODR_50_HZ);

  pinMode(burnwire_pin, OUTPUT);
  pinMode(buzzer_pin, OUTPUT);

  openLog.begin(9600);
  // xbee.begin(115200);
  gps_serial.begin(4800);


  if (!aht.begin())
    Serial.println("Trouble starting AHT");
  if (!bmp.begin_I2C())
    Serial.println("Trouble starting BMP");
  if (!bno.begin())
    Serial.println("Trouble starting BNO");
  // if (!rtc.begin())
  //   Serial.println("Trouble starting RTC");
  // if (!ccs.begin())
  //   Serial.println("Trouble starting CCS");
  if (!INA.begin())
    Serial.println("Trouble starting INA");

  
  while(!gps_serial){
    Serial.println("gpsssss");
  }
    ;

  // rtc.adjust(DateTime(2025, 4, 13, 0, 0, 0));
}

void set_ref_pressure() {
  for (int j = 0; j < 5; j++) {
    float readpressure = bmp.pressure / 100.0;
    while (readpressure < 100) {
      readpressure = bmp.pressure;
    }
    reference_pressure += readpressure;
  }
  reference_pressure /= 5;
}

bool calibrate_bmp() {
  reference_pressure = 0;
  bool bmp_calibrated = false;
  int i = 0, threshold = 5;
  while (i++ < 50) {
    set_ref_pressure();

    if (abs(bmp.pressure - reference_pressure) <= threshold) {
      bmp_calibrated = true;
      if (threshold > 0)
        threshold--;
    }
    reference_pressure = SEALEVELPRESSURE_HPA;
  }
  return bmp_calibrated;
}

void set_ref_orientation() {
  for (int j = 0; j < 5; j++) {
    reference_orientation_x += read_orientation_x();
    reference_orientation_y += read_orientation_y();
    reference_orientation_z += read_orientation_z();
  }

  reference_orientation_x /= 5;
  reference_orientation_y /= 5;
  reference_orientation_z /= 5;
}

bool calibrate_bno() {
  bool bno_calibrated = false;
  int i = 0, threshold = 10;

  while (i++ < 50) {
    set_ref_orientation();

    if (((abs(read_orientation_x() - reference_orientation_x) < threshold) &&
         (abs(read_orientation_y() - reference_orientation_y) < threshold) &&
         (abs(read_orientation_z() - reference_orientation_z) < threshold))) {

      adafruit_bno055_offsets_t bnoOffsets;

      bnoOffsets.gyro_offset_x = reference_orientation_x;
      bnoOffsets.gyro_offset_y = reference_orientation_y;
      bnoOffsets.gyro_offset_z = reference_orientation_z;

      bno.setSensorOffsets(bnoOffsets);

      bno_calibrated = true;

      if (threshold > 0)
        threshold--;
    }

    reference_orientation_x = 0;
    reference_orientation_y = 0;
    reference_orientation_z = 0;
  }

  return bno_calibrated;
};

// RTC reading
String read_time() {
  // DateTime now = rtc.now();

  // String time_string = time_formatting(now.hour()) + ":" +
  //                      time_formatting(now.minute()) + ":" +
  //                      time_formatting(now.second());
  // return time_string;

  return "00:00:00";
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
float read_velocity() { return get_velocity(); }

// GNSS reading
String read_gnss_time() {
  gps.encode(gps_serial.read());
  String time_string = (time_formatting(gps.time.hour()) + ":" +
                        time_formatting(gps.time.minute()) + ":" +
                        time_formatting(gps.time.second()));
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
  return gps.satellites.value();
  // return 0;
};

void correct_range_orientation(float *orientation_val) {
  float range_correction = ((*orientation_val > 180) ? 360 : 0);
  *orientation_val = *orientation_val - range_correction;
}
// BNO orientation reading
float read_orientation_x() {
  bno.getEvent(&orientationData, Adafruit_BNO055::VECTOR_EULER);
  correct_range_orientation(&orientationData.orientation.x);
  // return 0;

  return (orientationData.orientation.x);
};
float read_orientation_y() {
  correct_range_orientation(&orientationData.orientation.y);
  // return 0;

  return (orientationData.orientation.y);
};
float read_orientation_z() {
  correct_range_orientation(&orientationData.orientation.z);
  // return 0;
  return (orientationData.orientation.z);
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
  // return ccs.geteCO2();
  return 0;
};

void trigger_burnwire() {
  int i = 10;

  while (i--) {
    digitalWrite(burnwire_pin, HIGH);
    delay(250);
    digitalWrite(burnwire_pin, LOW);
    delay(50);
  }
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

bool sfw3_ready() {
  return (telemetry_data.velocity > 0 && telemetry_data.altitude > 0 &&
          telemetry_data.gnss_altitude > 0);
}
bool sfw4_ready() {
  return (telemetry_data.altitude >= 1000 || telemetry_data.velocity <= 0);
}
bool sfw6_ready() {
  return (velocity == 0 || (int)((telemetry_data.altitude) / 1) == 0);
}

bool is_fresh_start() { return (get_last_openlog_log().length() == 0); }

bool calibrate_sensors() { return (calibrate_bmp() && calibrate_bno()); }

void write_to_openlog(String telemetry_str) { openLog.println(telemetry_str); }
void send_thru_xbee(String telemetry_str) { xbee.print(telemetry_str); }
String read_from_xbee() {
  // String read_command = xbee.readString();
  String read_command = "";
  read_command.trim();
  // Serial.println(read_command);
  return read_command;
}

String get_last_openlog_log() {
  String logs = openLog.readString();
  logs.trim();

  int first_index = logs.indexOf(",");

  while (first_index) {
    first_index = logs.indexOf(",");
    logs = logs.substring(first_index);
  }

  return logs;
}

String get_last_state_string() {
  String last_log = get_last_openlog_log();

  int end_index = last_log.lastIndexOf(",");
  last_log = last_log.substring(0, end_index);

  int start_index = last_log.lastIndexOf(",");
  return last_log.substring(start_index + 1);
}
int get_last_state_num() {
  return state_stringToNum(prev_telemetry_data.software_state);
}

void start_telemetry() { send_telemetry_flag = true; }

void stop_telemetry() { send_telemetry_flag = false; }

bool telemetry_state() { return true; }
