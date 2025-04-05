#include <Adafruit_Sensor.h>
// #include <SparkFun_BMP581_Arduino_Library.h>
#include <RTClib.h>
#include <TinyGPS++.h>
#include <Wire.h>
#include <math.h>

// #include <PID_v1.h>                 // Include PID library
#include <Arduino.h>
// #include <SimpleKalmanFilter.h>
#include <Adafruit_BNO055.h>
// #include <utility/imumaths.h>
#include <Adafruit_AHTX0.h>
// #include <uEEPROMLib.h>
#include <Servo.h>

#include <SoftwareSerial.h>

#define burn_pin A3
#define cam_pin A0
#define servoPin1 10
#define servoPin2 11

SoftwareSerial Ser2(1, 0);
SoftwareSerial Ser1(9, 8);

float roll, pitch, yaw, recalt, gnssalt, c, ReferencePressure, b, velocity,
    altitude, alticmp, anet, volt = 0.0, velo, velo_avg, pressure, kalman_pres,
                             del_roll, deltaroll, hum;

unsigned long milli50, volt_sim, milli500, c50, Time, previousMillis;
unsigned int rec = 0, count = 0, fswstate = 0, value, count1 = 0, hour, minute,
             pwmval1, logerror = 0, restart = 0, pwmval2, state = 4,
             bckupcount = 0, packetcount = 1;

String tele, timestamp; // In gyrospin array, convert RPM to DPS
static bool wasPositive1 = true, wasPositive2 = true, done = false;
bool cam = false;
int gyrospin1, gyrospin2;

double input1, output1, input2, output2, setpoint = 0, Kp1 = 4.5, Ki1 = 0.03,
                                         Kd1 = 1.5, Kp2 = 4.5, Ki2 = 0.03,
                                         Kd2 = 1.5;

typedef struct flashStruct {
  int r_count;
  int r_fswstate;
  int r_packetcount;
  float r_ReferencePressure;
  float r_gnssalt;
  int r_rec;
} flashPrefs;

flashPrefs backup;
BMP581 BARO;
RTC_DS3231 rtc;
TinyGPSPlus gps;
Adafruit_BNO055 bno = Adafruit_BNO055(55, 0x28);
Adafruit_AHTX0 aht;
Servo servo1, servo2;
bmp5_sensor_data data = {0, 0};

sensors_event_t orientationData, accelerometerData, humidity, temp;
PID PID_controller1(&input1, &output1, &setpoint, Kp1, Ki1, Kd1, DIRECT);
PID PID_controller2(&input2, &output2, &setpoint, Kp2, Ki2, Kd2, DIRECT);
uEEPROMLib eeprom(0x50);

/*
SimpleKalmanFilter(e_mea, e_est, q);
 e_mea: Measurement Uncertainty
 e_est: Estimation Uncertainty
 q: Process Noise                      // Adjust q for faster response
 */
// SimpleKalmanFilter pressureKalmanFilter(0.9, 0.8, 0.9);

void setup() {

  // Serial.begin(115200);
  Ser1.begin(9600);
  Ser2.begin(115200);        // 115200 for Xbee Pro
  pinMode(cam_pin, OUTPUT);  // Camera
  pinMode(burn_pin, OUTPUT); // Burnwire
  pinMode(15, OUTPUT);       // Buzzer

  digitalWrite(cam_pin, LOW);
  digitalWrite(burn_pin, LOW);
  digitalWrite(15, LOW);

  Wire.setSDA(12);
  Wire.setSCL(13);
  Wire.begin();
  Wire.setClock(400000);

  byte sensor_status[4] = {1, 1, 1, 1};

  if (!rtc.begin())
    sensor_status[0] = 0;

  if (!bno.begin()) // Initialize IMU sensor
    sensor_status[1] = 0;

  if (BARO.beginI2C(BMP581_I2C_ADDRESS_DEFAULT)) // Initialize Pressure sensor
    sensor_status[2] = 0;

  if (!aht.begin())
    sensor_status[3] = 0;

  String sensors = String(sensor_status[0]) + ',' + String(sensor_status[1]) +
                   ',' + String(sensor_status[2]) + ',' +
                   String(sensor_status[3]);

  bmp5_iir_config config = {
      .set_iir_t = BMP5_IIR_FILTER_COEFF_1, // Set filter coefficient
      .set_iir_p = BMP5_IIR_FILTER_COEFF_1, // Set filter coefficient
      .shdw_set_iir_t = BMP5_ENABLE, // Store filtered data in data registers
      .shdw_set_iir_p = BMP5_ENABLE, // Store filtered data in data registers
      .iir_flush_forced_en = BMP5_DISABLE // Flush filter in forced mode
  };

  BARO.setFilterConfig(&config);

  bmp5_osr_odr_press_config osrMultipliers = {
      .osr_t = BMP5_OVERSAMPLING_8X,
      .osr_p = BMP5_OVERSAMPLING_4X,
      0,
      0 // Unused values, included to avoid compiler warnings-as-error
  };

  BARO.setOSRMultipliers(&osrMultipliers);
  BARO.setODRFrequency(50);

  for (int i = 0; i < 5; i++) {
    BARO.getSensorData(&data);
    delay(100);
  }
  // buzz();
  // recover();

  // if (rtc.lostPower()) {
  if (1) {
    rtc.adjust(DateTime(2024, 4, 15, 0, 0, 0));
    BARO.getSensorData(&data);
    delay(100);

    backup.r_count = 0;
    backup.r_fswstate = 0;
    backup.r_packetcount = 0;
    backup.r_ReferencePressure = 0;
    backup.r_gnssalt = 0;
    backup.r_rec = 0;

    eeprom.eeprom_write(8, &backup, sizeof(backup));
    recover();

    while (!(Ser2.read() == 49))
      ; //(!((Ser2.read()) == '3'|| (Serial.read()=='3')))

    Ser2.println(sensors);

    for (int i = 0; i < 50; i++) {
      BARO.getSensorData(&data);
      b += data.pressure;
      delay(100);
    }
    ReferencePressure = b / 50;
    b = 0;
  }
}

void setup1() {
  servo1.attach(servoPin1);
  servo1.writeMicroseconds(1500);
  delay(2000);

  servo2.attach(servoPin2);
  servo2.writeMicroseconds(1500);
  delay(2000);

  PID_controller1.SetMode(AUTOMATIC);
  PID_controller1.SetTunings(Kp1, Ki1, Kd1);
  PID_controller1.SetOutputLimits(-300, 300);

  PID_controller2.SetMode(AUTOMATIC);
  PID_controller2.SetTunings(Kp2, Ki2, Kd2);
  PID_controller2.SetOutputLimits(-300, 300);
}

void loop() {

  aht.getEvent(&humidity, &temp);

  bno.getEvent(&orientationData, Adafruit_BNO055::VECTOR_EULER);
  bno.getEvent(&accelerometerData, Adafruit_BNO055::VECTOR_ACCELEROMETER);

  roll = orientationData.orientation.y;
  pitch = orientationData.orientation.z;
  yaw = orientationData.orientation.x;

  //-------------------------------------------------------------------------------
  if (fswstate > 2)
    done = true;
  //-------------------------------------------------------------------------------
  fsw();
  runFSM();

  if (millis() - milli500 >= 1000) {
    milli500 += 1000;
    voltage();
    readrtc();

    if (cam == false) {
      camera();
      cam = true;
    }
    if (count == 0 &
        (Ser2.read() ==
         50)) //(count == 0 & ((Ser2.read()) == '3'|| (Serial.read()=='3')))
    {
      // burn();
      for (int i = 0; i < 50; i++) {
        BARO.getSensorData(&data);
        b += data.pressure;
        delay(100);
      }
      ReferencePressure = b / 50;
      gnssalt = gps.altitude.meters();
      b = 0;
    }

    if (count > 0 & altitude > 150 & altitude < 1000 & (Ser2.read() == 50)) {
      burn();
    }

    if (abs(output1) < 50)
      gyrospin1 = 0;
    else
      gyrospin1 = 90 * output1;

    if (abs(output2) < 50)
      gyrospin2 = 0;
    else
      gyrospin2 = 90 * output2;

    hour = gps.time.hour() + 5;
    minute = gps.time.minute() + 30;

    if (minute > 59) {
      minute = minute - 60;
      hour++;
    }
    if (hour > 23)
      hour = hour - 24;

    unsigned int sats = gps.satellites.value();
    float gnss_alt_tele = gps.altitude.meters();
    hum = humidity.relative_humidity - 3;

    if (sats > 50)
      sats = 7;

    tele = String("2022ASI-050") + ',' + timestamp + ',' + String(packetcount) +
           ',' + String(altitude, 1) + ',' + String(pressure, 0) + ',' +
           String(temp.temperature, 1) + ',' + String(volt) + ',' +
           String(hour) + ':' + String(minute) + ':' +
           String(gps.time.second()) + ',' + String(gps.location.lat(), 4) +
           ',' + String(gps.location.lng(), 4) + ',' +
           String(gnss_alt_tele, 1) + ',' + String(sats) + ',' +
           String(accelerometerData.acceleration.y) + ',' +
           String(accelerometerData.acceleration.z) + ',' +
           String(accelerometerData.acceleration.x) + ',' + String(roll) + ',' +
           String(pitch) + ',' + String(yaw) + ',' + String(gyrospin1) + ',' +
           String(gyrospin2) + ',' + String(fswstate) + ',' + String(hum) +
           ',' + String(velocity);

    Ser2.println(tele);
    Ser1.println(tele);
    // Serial.println(tele);

    /*      if (bckupcount == 4) {
           backup.r_count = count;
           backup.r_fswstate = fswstate;
           backup.r_packetcount = packetcount;
           backup.r_ReferencePressure = ReferencePressure ;
           backup.r_gnssalt= gnssalt;
           backup.r_rec=rec;
           eeprom.eeprom_write(8, &backup, sizeof(backup));
           bckupcount = 0;
         }
    */
    // bckupcount++;

    packetcount++;
    // count1 = 0;
  }
}

void loop1() {

  smartDelay(0);

  if (done == true) {
    pid_new();
    done = false;
  }
}

void burn() {
  // Serial.println("BURN");
  digitalWrite(burn_pin, HIGH);
  previousMillis = millis();
  state = 0;
}

void voltage() {
  value = analogRead(A2);
  volt = (value * 3.3) /
         (0.2 * 1023.0); // Take 5.0 V in UNO and 3.3 V in Nano rp2040

  // declare volt = 16.6 in global variable
  /*if(millis() - volt_sim > 60000) {
    volt_sim += millis();
    volt -= 0.05;
  }*/
}

void readrtc() {
  // January 21, 2014 at 3am you would call:
  // rtc.adjust(DateTime(2014, 1, 21, 3, 0, 0));
  DateTime now = rtc.now();
  timestamp = String(now.hour()) + ':' + String(now.minute()) + ':' +
              String(now.second());
}

void buzz() {
  // Serial.println("BUZZ");
  while (1) {
    // buzz_ser.print("11111111111111111100000000000000000000");
    // delay(10);
    digitalWrite(15, HIGH);
    delay(1);
    digitalWrite(15, LOW);
    delay(1);
  }
}

void recover() {
  eeprom.eeprom_read(8, &backup);
  count = backup.r_count;
  fswstate = backup.r_fswstate;
  packetcount = backup.r_packetcount + 1;
  ReferencePressure = backup.r_ReferencePressure;
  gnssalt = backup.r_gnssalt;
  rec = backup.r_rec;
}

void simulate() {
  if (c == 0)
    altitude += 80;

  if (c == 1 | altitude > 850) {
    c = 1;
    altitude -= 15;
  }

  if ((c == 1 & altitude < 500) | c == 2) {
    altitude -= 3;
    c = 2;
  }
  // if (altitude<-5)
  // Serial.println("------------------------------------------------------------------------------------------");
}

/*void fsw() {

  if (millis() - c50 >= 250) {

    c50 += 250;
    BARO.getSensorData(&data);
    //kalman_pres = pressureKalmanFilter.updateEstimate(BARO.pressure);
    pressure = data.pressure + 800;
    altitude = 44330 * (1 - pow(data.pressure / ReferencePressure, 0.1903));
    //simulate();
    //velocity = pressureKalmanFilter.updateEstimate(10 * (altitude - alticmp));
    velo = 4 * (altitude - alticmp);
    //velocity = 10 * (altitude - alticmp);
    alticmp = altitude;
    velo_avg += velo;
    count1++;

    if (count1 == 4) {
    velocity = velo_avg / count1;
    velo_avg = 0;
    count1 = 0;
    }
    del_roll = deltaroll - roll;
    deltaroll = roll;

    recalt=gps.altitude.meters()-gnssalt;

    if(altitude>1100 & altitude<1500){
    altitude=recalt;
    rec=1;
    }

    if(rec==1)
    altitude=recalt;

    if (abs(velocity)>120)
    altitude=alticmp;

  // Serial.print("0");  //Launch pad
  //  if (altitude > 10 & altitude < 600 & velocity>-0.1 & fswstate <= 1) {

  if (altitude > 10 & altitude < 20 & fswstate == 0) {
    //Serial.print("1");  //Ascent  //NOTE: Calibrate Accelerometer for JERKs,
i.e. discard values greater than 9.8 m/s2 fswstate = 1; count = 1;
  }

  else if ((altitude > 55 & altitude<80 & velocity>-0.1 & count == 1) ||
(velocity < -8 & fswstate==1)) {
    //Serial.print("2");  //Rocket Deploy
    fswstate = 2;
    if (count == 1) {
      count = 2;
      //camera();
    }
  }

  else if (velocity<-8 & altitude > 54 & altitude < 80 & count <= 2) {
    //Serial.print("3");  //Descent
    fswstate = 3;
    if (accelerometerData.acceleration.x < -10) {
      fswstate = 4;  //P1 deploy
      //Serial.print("4");
      count = 3;

      tele = String("2022ASI-050") + ',' + timestamp + ',' + String(packetcount)
+ ',' + String(altitude, 1) + ',' + String(pressure, 0) + ','
            + String(temp.temperature, 1) + ',' + String(volt) + ',' +
String(hour) + ':' + String(minute) + ':' + String(gps.time.second())
            + ',' + String(gps.location.lat(), 4) + ',' +
String(gps.location.lng(), 4) + ',' + String(gps.altitude.meters(), 1) + ',' +
String(gps.satellites.value())
            + ',' + String(accelerometerData.acceleration.y) + ',' +
String(accelerometerData.acceleration.z) + ',' +
String(accelerometerData.acceleration.x) + ',' + String(roll) + ',' +
String(pitch) + ',' +  String(yaw) + ',' + String(gyrospin1) + ',' +
String(gyrospin2) + ',' + String(fswstate)
            + ',' + String(humidity.relative_humidity) + ',' + String(velocity);

      Ser2.println(tele);
      Ser1.println(tele);
      //Serial.println(tele);
    }
  }

  else if (fswstate>=3 & count < 5 & velocity<0 & altitude > 25 & altitude < 53)
{
    //Serial.print("5");  //Descent Stage 2
    fswstate = 5;
    if (count < 4) {
      count = 4;
      burn();
    }
    if (accelerometerData.acceleration.x < -10) {
      fswstate = 6;  //P2 deploy
      // Serial.print("6");
      count = 5;

      tele = String("2022ASI-050") + ',' + timestamp + ',' + String(packetcount)
+ ',' + String(altitude, 1) + ',' + String(pressure, 0) + ','
            + String(temp.temperature, 1) + ',' + String(volt) + ',' +
String(hour) + ':' + String(minute) + ':' + String(gps.time.second())
            + ',' + String(gps.location.lat(), 4) + ',' +
String(gps.location.lng(), 4) + ',' + String(gps.altitude.meters(), 1) + ',' +
String(gps.satellites.value())
            + ',' + String(accelerometerData.acceleration.y) + ',' +
String(accelerometerData.acceleration.z) + ',' +
String(accelerometerData.acceleration.x) + ',' + String(roll) + ',' +
String(pitch) + ',' +  String(yaw) + ',' + String(gyrospin1) + ',' +
String(gyrospin2) + ',' + String(fswstate)
            + ',' + String(humidity.relative_humidity) + ',' + String(velocity);

      Ser2.println(tele);
      Ser1.println(tele);
      //Serial.println(tele);
    }
  }

  else if (count > 2 & altitude < 10 & abs(del_roll) < 2)  //Difference Of roll
  {
    // Serial.print("7");  //Touchdown
    fswstate = 7;
    //Stop all sensors
    if (count < 6) {
      count = 6;
      previousMillis = millis();
    }

    if (millis() - previousMillis > 30000) {

      bno.enterSuspendMode();
      Ser2.end();
      Ser1.end();

      digitalWrite(cam_pin, HIGH);  //Stop Recording
      delay(50);
      digitalWrite(cam_pin, LOW);

      servo1.writeMicroseconds(1512);  // Send signal for braking
      servo2.writeMicroseconds(1512);

      buzz();
    }
  }
 }
}
*/
void fsw() {

  if (millis() - c50 >= 250) {

    c50 += 250;
    BARO.getSensorData(&data);
    // kalman_pres = pressureKalmanFilter.updateEstimate(BARO.pressure);
    pressure = data.pressure + 500;
    altitude = 44330 * (1 - pow(data.pressure / ReferencePressure, 0.1903));
    // simulate();
    // velocity = pressureKalmanFilter.updateEstimate(10 * (altitude -
    // alticmp));
    velo = 4 * (altitude - alticmp);
    // velocity = 10 * (altitude - alticmp);
    alticmp = altitude;
    velo_avg += velo;
    count1++;

    if (count1 == 4) {
      velocity = velo_avg / count1;
      velo_avg = 0;
      count1 = 0;
    }

    del_roll = deltaroll - roll;
    deltaroll = roll;

    recalt = gps.altitude.meters() - gnssalt;

    if (altitude > 1100 & altitude < 1500) {
      altitude = recalt;
      rec = 1;
    }

    if (rec == 1)
      altitude = recalt;

    if (abs(velocity) > 120)
      altitude = alticmp;

    // Serial.print("0");  //Launch pad
    //  if (altitude > 10 & altitude < 600 & velocity>-0.1 & fswstate <= 1) {

    if (altitude > 10 & altitude < 300 & fswstate == 0) {
      // Serial.print("1");  //Ascent  //NOTE: Calibrate Accelerometer for
      // JERKs, i.e. discard values greater than 9.8 m/s2
      fswstate = 1;
      count = 1;
    }

    else if ((altitude > 700 & altitude<950 & velocity> - 0.1 & count == 1) ||
             (velocity < -8 & fswstate == 1)) {
      // Serial.print("2");  //Rocket Deploy
      fswstate = 2;
      if (count == 1) {
        count = 2;
        // camera();
      }
    }

    else if (velocity < -8 & altitude > 551 & altitude < 1000 & count <= 2) {
      // Serial.print("3");  //Descent
      fswstate = 3;
      if (accelerometerData.acceleration.x < -10) {
        fswstate = 4; // P1 deploy
        // Serial.print("4");
        count = 3;

        tele = String("2022ASI-050") + ',' + timestamp + ',' +
               String(packetcount) + ',' + String(altitude, 1) + ',' +
               String(pressure, 0) + ',' + String(temp.temperature, 1) + ',' +
               String(volt) + ',' + String(hour) + ':' + String(minute) + ':' +
               String(gps.time.second()) + ',' + String(gps.location.lat(), 4) +
               ',' + String(gps.location.lng(), 4) + ',' +
               String(gps.altitude.meters(), 1) + ',' +
               String(gps.satellites.value()) + ',' +
               String(accelerometerData.acceleration.y) + ',' +
               String(accelerometerData.acceleration.z) + ',' +
               String(accelerometerData.acceleration.x) + ',' + String(roll) +
               ',' + String(pitch) + ',' + String(yaw) + ',' +
               String(gyrospin1) + ',' + String(gyrospin2) + ',' +
               String(fswstate) + ',' + String(hum) + ',' + String(velocity);

        Ser2.println(tele);
        Ser1.println(tele);
        // Serial.println(tele);
      }
    }

    else if (fswstate >= 3 & count < 5 & velocity < 0 & altitude > 400 &
             altitude < 550) {
      // Serial.print("5");  //Descent Stage 2
      fswstate = 5;
      if (count < 4) {
        count = 4;
        burn();
      }
      if (accelerometerData.acceleration.x < -10) {
        fswstate = 6; // P2 deploy
        // Serial.print("6");
        count = 5;

        tele = String("2022ASI-050") + ',' + timestamp + ',' +
               String(packetcount) + ',' + String(altitude, 1) + ',' +
               String(pressure, 0) + ',' + String(temp.temperature, 1) + ',' +
               String(volt) + ',' + String(hour) + ':' + String(minute) + ':' +
               String(gps.time.second()) + ',' + String(gps.location.lat(), 4) +
               ',' + String(gps.location.lng(), 4) + ',' +
               String(gps.altitude.meters(), 1) + ',' +
               String(gps.satellites.value()) + ',' +
               String(accelerometerData.acceleration.y) + ',' +
               String(accelerometerData.acceleration.z) + ',' +
               String(accelerometerData.acceleration.x) + ',' + String(roll) +
               ',' + String(pitch) + ',' + String(yaw) + ',' +
               String(gyrospin1) + ',' + String(gyrospin2) + ',' +
               String(fswstate) + ',' + String(hum) + ',' + String(velocity);

        Ser2.println(tele);
        Ser1.println(tele);
        // Serial.println(tele);
      }
    }

    else if (count > 2 & altitude < 50 &
             abs(del_roll) < 2) // Difference Of roll
    {
      // Serial.print("7");  //Touchdown
      fswstate = 7;
      // Stop all sensors
      if (count < 6) {
        count = 6;
        previousMillis = millis();
      }

      if (millis() - previousMillis > 60000) {

        bno.enterSuspendMode();
        Ser2.end();
        Ser1.end();

        digitalWrite(cam_pin, HIGH); // Stop Recording
        delay(50);
        digitalWrite(cam_pin, LOW);

        servo1.writeMicroseconds(1512); // Send signal for braking
        servo2.writeMicroseconds(1512);

        buzz();
      }
    }
  }
}

void pid_new() {

  input1 = -1 * roll; // set offset
  input2 = pitch;

  bool isPositive1 = (input1 > 0);
  if (wasPositive1 != isPositive1) {
    servo1.writeMicroseconds(1512); // Send signal for braking
    wasPositive1 = isPositive1;     // Update the previous sign
    delay(50);
  }

  PID_controller1.Compute();
  servo1.writeMicroseconds(output1 + 1512); // Send signal to ESC.

  //-----------------------------------------------------------------------

  bool isPositive2 = (input2 > 0);
  if (wasPositive2 != isPositive2) {
    servo2.writeMicroseconds(1512); // Send signal for braking
    wasPositive2 = isPositive2;     // Update the previous sign
    delay(50);
  }

  PID_controller2.Compute();
  servo2.writeMicroseconds(output2 + 1512); // Send signal to ESC.
}

void camera() {
  // Serial.println("CAM");
  digitalWrite(cam_pin, HIGH);
  previousMillis = millis();
  state = 1;
}

void runFSM() {

  unsigned long currentMillis = millis();

  switch (state) {

  case 0: // Burn
    if (currentMillis - previousMillis > 3000) {
      digitalWrite(burn_pin, LOW);
      previousMillis = currentMillis;
      state = 4;
    }
    break;
  case 1: // Turn on camera
    if (currentMillis - previousMillis > 2100) {
      digitalWrite(cam_pin, LOW);
      previousMillis = currentMillis;
      state++;
    }
    break;
  case 2: // Start recording
    if (currentMillis - previousMillis > 2500) {
      digitalWrite(cam_pin, HIGH);
      previousMillis = currentMillis;
      state++;
    }
    break;
  case 3: // Toggle
    if (currentMillis - previousMillis > 50) {
      digitalWrite(cam_pin, LOW);
      previousMillis = currentMillis;
      state++;
    }
    break;
  default:
    // Do nothing
    break;
  }
}

static void smartDelay(unsigned long ms) {
  unsigned long start = millis();
  // do {
  while (Ser1.available())
    gps.encode(Ser1.read());
  //} while (millis() - start < ms);
}
