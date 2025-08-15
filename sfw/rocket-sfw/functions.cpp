#include "functions.h"

#define SEALEVELPRESSURE_HPA (1013.25)

bool send_telemetry_flag = false;

Adafruit_AHTX0 aht;
Adafruit_BMP3XX bmp;
Adafruit_BNO055 bno = Adafruit_BNO055(55);
TinyGPSPlus gps;
SoftwareSerial gps_serial(9, 8);
RTC_DS3231 rtc;
Adafruit_CCS811 ccs;
INA226 INA(0x40);
SoftwareSerial openLog(1, 0);
// SoftwareSerial xbee(13, 12);


float reference_pressure = SEALEVELPRESSURE_HPA;
float reference_orientation_x = 0, reference_orientation_y = 0,
      reference_orientation_z = 0;
sensors_event_t humidity, temp, orientationData, accelerationData;
String last_log;
int packet_count = 0;

bool calibrate_bmp();
bool calibrate_bno();

void setup_sensors() {
  while (!Serial) {}

  bmp.setTemperatureOversampling(BMP3_OVERSAMPLING_8X);
  bmp.setPressureOversampling(BMP3_NO_OVERSAMPLING);
  bmp.setIIRFilterCoeff(BMP3_IIR_FILTER_COEFF_127);
  bmp.setOutputDataRate(BMP3_ODR_200_HZ);

  pinMode(burnwire_pin, OUTPUT);
  pinMode(buzzer_pin, OUTPUT);

  openLog.begin(9600);
  // xbee.begin(115200);
  gps_serial.begin(4800);


  if (!aht.begin()) {
    Serial.println("Trouble starting AHT");
    set_component_status("aht", false);
  }
  if (!bmp.begin_I2C()) {
    Serial.println("Trouble starting BMP");
    set_component_status("bmp", false);
  }
  if (!bno.begin()) {
    Serial.println("Trouble starting BNO");
    set_component_status("bno", false);
  }
  if (!rtc.begin()) {
    Serial.println("Trouble starting RTC");
    set_component_status("rtc", false);
  }
  if (!ccs.begin()) {
    Serial.println("Trouble starting CCS");
    set_component_status("ccs", false);
  }
  if (!INA.begin()) {
    Serial.println("Trouble starting INA");
    set_component_status("ina", false);
  }
  while (!gps_serial) {
    Serial.println("No gps");
    set_component_status("gps", false);
  };

  // LoRa Setup
  LoRa.setSPI(SPI1);
  LoRa.setPins(SS_PIN, RST_PIN, DIO0_PIN);
  LoRa.setTxPower(TX_POWER);
  LoRa.setSignalBandwidth(BANDWIDTH);
  LoRa.setSpreadingFactor(SPREADING_FACTOR);
  LoRa.setCodingRate4(CODING_RATE);
  LoRa.setSyncWord(SYNC_WORD);



  // rtc.adjust(DateTime(2025, 4, 13, 0, 0, 0));

  // OpenLog setup
  {
    // SD card config:
    // 9600,26,3,1,1,1,0
    // baud,escape,esc#,mode,verb,echo,ignoreRX

    openLog.write(26);
    openLog.write(26);
    openLog.write(26);

    openLog.print("append ");
    openLog.print("new_log.csv\r");
    int i = 10;
    while (i) {
      if (openLog.available()) {
        if (openLog.read() == '>') {
          Serial.println("OpenLog Ready!");
          break;
        }
      } else {
        i--;
      }
    }
    if (!i) {
      Serial.println("Trouble starting OpenLog");
      set_component_status("openlog", false);
    };
  }
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
  while (i++ < 10) {
    set_ref_pressure();

    if (abs(bmp.readAltitude(reference_pressure)) <= threshold) {
      bmp_calibrated = true;
      return bmp_calibrated;
    }

    delay(100);
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

    if (((abs(read_orientation_x() - reference_orientation_x) < threshold) && (abs(read_orientation_y() - reference_orientation_y) < threshold) && (abs(read_orientation_z() - reference_orientation_z) < threshold))) {

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
int read_packet_count() {
  return packet_count++;
}

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
float read_velocity() {
  return get_velocity();
}

// GNSS reading
String read_gnss_time() {
  gps.encode(gps_serial.read());
  String time_string = (time_formatting(gps.time.hour()) + ":" + time_formatting(gps.time.minute()) + ":" + time_formatting(gps.time.second()));
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
  return (orientationData.orientation.x);

  // return 0;
};
float read_orientation_y() {
  correct_range_orientation(&orientationData.orientation.y);

  // return 0;

  return (orientationData.orientation.y);
};
float read_orientation_z() {
  correct_range_orientation(&orientationData.orientation.z);
  return (orientationData.orientation.z);

  // return 0;
};

// BNO acceleration reading
float read_acceleration_x() {
  bno.getEvent(&accelerationData, Adafruit_BNO055::VECTOR_ACCELEROMETER);
  return accelerationData.acceleration.x;

  // return 0;
};

float read_acceleration_y() {
  return accelerationData.acceleration.y;

  // return 0;
};
float read_acceleration_z() {
  return accelerationData.acceleration.z;

  // return 0;
};

// Software State Reading
String read_software_state() {
  return get_software_state_string();
};

// MiCS reading (CO/CH4/C2H5OH/H2/NH3)
float read_gas_concentration() {
  // return ccs.geteCO2();

  return 0;
};

void trigger_burnwire() {
  digitalWrite(burnwire_pin, HIGH);
  delay(4000);
  digitalWrite(burnwire_pin, LOW);
  delay(200);
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
  return (telemetry_data.velocity > 0 && telemetry_data.altitude > 0 && telemetry_data.gnss_altitude > 0);
}
bool sfw4_ready() {
  return (telemetry_data.altitude >= 980 || telemetry_data.velocity <= 0);
}
bool sfw6_ready() {
  return (telemetry_data.velocity < 1 || (int)(telemetry_data.altitude / 1) == 0);
}

bool is_fresh_start() {
  return (get_last_openlog_log().length() == 0);
}

bool calibrate_sensors() {
  return (calibrate_bmp() && calibrate_bno());
}

void write_to_eeprom(telemetry_data_struct telemetry_data) {
  EEPROM.write(0, state_stringToNum(telemetry_data.software_state));
}
int read_from_eeprom() {
  return EEPROM.read(0);
}

void write_to_openlog(String telemetry_str) {
  openLog.print(telemetry_str);
  openLog.print("\r");
}
void send_thru_xbee(String telemetry_str) {
  // xbee.println(telemetry_str);
}
String read_from_xbee() {
  // if (xbee.available()) {
  //   String command = xbee.readString();
  //   command.trim();
  // }
  String command = "";
  return command;
}

void send_to_eeprom() {
  // EEPROM.write
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

void start_telemetry() {
  send_telemetry_flag = true;
}

void stop_telemetry() {
  send_telemetry_flag = false;
}

bool telemetry_state() {
  return send_telemetry_flag;
  return true;
}
