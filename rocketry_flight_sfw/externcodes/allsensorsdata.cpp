#include <Adafruit_AHTX0.h>
#include <Adafruit_BMP3XX.h>
#include <Adafruit_BNO055.h>
#include <Adafruit_Sensor.h>
#include <DFRobot_MICS.h>
#include <INA226.h>
#include <RTClib.h>
#include <SoftwareSerial.h>
#include <TinyGPS++.h>
#include <Wire.h>
#include <utility/imumaths.h>

#define SEALEVELPRESSURE_HPA (1013.25)
#define MICS_I2C_ADDRESS MICS_ADDRESS_0 // 0x75

Adafruit_AHTX0 aht;
Adafruit_BMP3XX bmp;
Adafruit_BNO055 bno = Adafruit_BNO055(55, 0x28, &Wire);
TinyGPSPlus gps;
RTC_DS3231 rtc;
DFRobot_MICS_I2C mics(&Wire, MICS_I2C_ADDRESS);
INA226 INA(0x40);

volatile long prevMillis = 0;
double lat, lon, alt;
int sats, hrs, mins, secs;

void setup() {

  Serial.begin(115200);
  Wire.setSDA(4);
  Wire.setSCL(5);
  Wire.begin();
  Wire.setClock(400000);

  bmp.setTemperatureOversampling(BMP3_OVERSAMPLING_8X);
  bmp.setPressureOversampling(BMP3_OVERSAMPLING_4X);
  bmp.setIIRFilterCoeff(BMP3_IIR_FILTER_COEFF_3);
  bmp.setOutputDataRate(BMP3_ODR_50_HZ);

  bno.begin();
  aht.begin();
  bmp.begin_I2C();
  rtc.begin();
  mics.begin();
  INA.begin();
}

void setup1() { Serial.begin(115200); }

void loop() {

  unsigned long currMillis = millis();

  // Declaring sensor events
  sensors_event_t humidity, temp, orientationData, accelerometerData;
  aht.getEvent(&humidity, &temp);
  bno.getEvent(&orientationData, Adafruit_BNO055::VECTOR_EULER);
  bno.getEvent(&accelerometerData, Adafruit_BNO055::VECTOR_ACCELEROMETER);

  String teamID = "2024 ASI-026"; // Team-ID

  // Time stamping
  unsigned long totalTime = millis() / 1000.0;
  DateTime now = rtc.now();
  Serial.print(now.hour(), DEC);
  Serial.print(':');
  Serial.print(now.minute(), DEC);
  Serial.print(':');
  Serial.print(now.second(), DEC);
  Serial.println();

  // Packet counts

  // Altitude
  Serial.print("Altitude = ");
  Serial.print(bmp.readAltitude(SEALEVELPRESSURE_HPA), 1);
  Serial.print("m | ");

  // Pressure
  Serial.print("Pressure = ");
  Serial.print(bmp.pressure, 0);
  Serial.print("Pa | ");

  // Temperature
  Serial.print("Temperature = ");
  Serial.print(temp.temperature, 1);
  Serial.print("°C | ");

  // Voltage
  Serial.print("Voltage = ");
  Serial.print(INA.getBusVoltage(), 2);
  Serial.print("V | ");

  // Gyro sensor & Accelerometer
  float roll = orientationData.orientation.y;
  float pitch = orientationData.orientation.z;
  float yaw = orientationData.orientation.x;

  float accelX = accelerometerData.acceleration.x;
  float accelY = accelerometerData.acceleration.y;
  float accelZ = accelerometerData.acceleration.z;

  Serial.print("Roll = ");
  Serial.print(roll, 2);
  Serial.print("° | ");
  Serial.print("Pitch = ");
  Serial.print(pitch, 2);
  Serial.print("° | ");
  Serial.print("Yaw = ");
  Serial.println(yaw, 2);
  Serial.print("° | ");

  Serial.print("Accn X = ");
  Serial.print(accelX, 2);
  Serial.print("m/s^2 | ");
  Serial.print("Accn Y = ");
  Serial.print(accelY, 2);
  Serial.print("m/s^2 | ");
  Serial.print("Accn Z = ");
  Serial.println(accelZ, 2);
  Serial.print("m/s^2 | ");

  // Gyro spin rate

  // Flight software state

  // Humidity
  Serial.print("Humidity: ");
  Serial.print(humidity.relative_humidity);
  Serial.println("% rH");

  // Gas data
  Serial.print("CO = ");
  Serial.print(mics.getGasData(CO), 2);
  Serial.print("PPM | ");

  Serial.print("CH4 = ");
  Serial.print(mics.getGasData(CH4), 2);
  Serial.print("PPM | ");

  Serial.print("C2H50H = ");
  Serial.print(mics.getGasData(C2H5OH), 2);
  Serial.print("PPM | ");

  Serial.print("H2 = ");
  Serial.print(mics.getGasData(H2), 2);
  Serial.print("PPM | ");

  Serial.print("NH3 = ");
  Serial.print(mics.getGasData(NH3), 2);
  Serial.println("PPM | ");
}

void loop1() {
  while (Serial1.available() > 0) {
    if (gps.encode(Serial1.read())) {
      double lat = gps.location.lat();
      double lon = gps.location.lng();
      double alt = gps.altitude.meters();
      int sats = gps.satellites.value();
      int hrs = gps.time.hour();
      int mins = gps.time.minute();
      int sec = gps.time.second();
    }

    Serial.print("GNSS Time = ");
    Serial.print(hrs);
    Serial.print(":");
    Serial.print(mins);
    Serial.print(":");
    Serial.print(secs);
    Serial.print(" | ");

    Serial.print("Latitude = ");
    Serial.print(lat, 4);
    Serial.print("° | ");

    Serial.print("Longitude = ");
    Serial.print(lon, 4);
    Serial.print("° | ");

    Serial.print("Altitude = ");
    Serial.print(alt, 1);
    Serial.print("m | ");

    Serial.print("Satellites = ");
    Serial.print(sats, 0);
    Serial.println(" | ");
  }
}
