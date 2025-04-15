#include "functions.h"
#include "SoftwareSerial.h"
#include "calculated_data.h"
#include "previous_data.h"

#define SEALEVELPRESSURE_HPA (1013.25)

Adafruit_AHTX0 aht;
Adafruit_BMP3XX bmp;
Adafruit_BNO055 bno = Adafruit_BNO055(55);
Adafruit_CCS811 ccs;
SoftwareSerial openLog(1, 0);
SoftwareSerial xbee(13, 12);


float reference_pressure = SEALEVELPRESSURE_HPA, reference_altitude = 0;
float reference_orientation_x = 0, reference_orientation_y = 0,
      reference_orientation_z = 0;
sensors_event_t humidity, temp, orientationData, accelerationData;


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

  openLog.begin(115200);
  xbee.begin(115200);

  if (!aht.begin())
    Serial.println("Trouble starting AHT");
  if (!bmp.begin_I2C())
    Serial.println("Trouble starting BMP");
  if (!bno.begin())
    Serial.println("Trouble starting BNO");
  if (!ccs.begin())
    Serial.println("Trouble starting CCS");
 
  calibrate_bmp();
  calibrate_bno();

  rtc.adjust(DateTime(2025, 4, 13, 0, 0, 0));
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

// BNO orientation reading
float read_orientation_x() {
  bno.getEvent(&orientationData, Adafruit_BNO055::VECTOR_EULER);
  orientationData.orientation.x -=
      ((orientationData.orientation.x > 180) ? 360 : 0);
  // return 0;
  return (orientationData.orientation.x);
};
float read_orientation_y() {
  orientationData.orientation.y -=
      ((orientationData.orientation.y > 180) ? 360 : 0);
  // return 0;

  return (orientationData.orientation.y);
};
float read_orientation_z() {
  orientationData.orientation.z -=
      ((orientationData.orientation.z > 180) ? 360 : 0);
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

// CCS reading (CO/CH4/C2H5OH/H2/NH3)
float read_gas_concentration() {
  return ccs.geteCO2();
  // return 0;
};

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

bool calibrate_sensors() {
  return (set_reference_alt_pressure() && set_reference_orientation());
}

void write_to_openlog(String telemetry_str) { openLog.println(telemetry_str); }

void send_thru_xbee(String telemetry_str) { xbee.print(telemetry_str); }

String read_from_xbee() {
  String readString = xbee.readString();
  readString.trim();
  return readString;
}
