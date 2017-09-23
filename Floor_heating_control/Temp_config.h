#ifndef TEMP_CONFIG
#define TEMP_CONFIG

#define ROOMS 5
#define ALL_SENSORS 13

#define BUSY_PIN 7    // pin of busy LED

#define ALARM_OFFSET sizeof(byte[8])+sizeof(char[20])
#define DELTA_OFFSET ALARM_OFFSET+sizeof(float)
#define NIGHT_OFFSET DELTA_OFFSET+sizeof(float)
#define RELAY_OFFSET NIGHT_OFFSET+sizeof(float)

#define MODE_OFF         0
#define MODE_ON          1
#define MODE_SCHEDULING  2

#define CHECK_INTERVAL 100000  //10mp
//#define CHECK_EINTELVAL 20000

unsigned long last_checked, last_checked_extra, _now_temp;
float alarms[ROOMS];
float nights[ROOMS];
byte relays[ROOMS];
byte current_mode;


struct TempSensor {
  byte addr[8];
  char name[20];
};

struct TempSensorData {
  byte addr[8];
  char name[20];
  float alarm;
  float delta;
  float night;
  byte relay;
};

TempSensorData sensor0 = {
      {0x28, 0x1A, 0xA9, 0xAE, 0x07, 0x00, 0x00, 0x8C},   //28.1AA9AE0700008C
      "konyha",
      20.00f,
      0.10f,
      1.00f,
      12
};

TempSensorData sensor1 = {
      {0x28, 0xFF, 0x17, 0x07, 0x64, 0x15, 0x03, 0x29},  //28.FF170764150329
      "|tkez#",
      20.34f,
      0.10f,
      18.11f,
      11
};
TempSensorData sensor2 = {
      {0x28, 0xFF, 0xB6, 0x38, 0x71, 0x15, 0x01, 0xC6},    //28.FFB638711501C6
      "f$rd#",
      21.00f,
      0.1f,
      0.00f,
      9
};
TempSensorData sensor3 = {
      {0x28, 0xFF, 0xA7, 0x7D, 0x71, 0x15, 0x01, 0x16},    //28.FFA77D71150116
      "el#szoba",
      20.00f,
      0.10f,
      5.25f,
      10
};
TempSensorData sensor4 = {
      {0x28, 0xFF, 0x24, 0xA6, 0x63, 0x15, 0x01, 0x03},    //28.FF24A663150103
      "wc",
      21.00f,
      0.10f,
      0.00f,
      8
};
TempSensor sensor5 = {
      {0x28, 0xFF, 0xCF, 0xE7, 0x63, 0x15, 0x01, 0xC6},   //28.FFCFE7631501C6
      "nappali"
};

TempSensor sensor6 = {
      {0x28, 0xFF, 0x4C, 0x07, 0x64, 0x15, 0x03, 0xCC},    //28.FF4C07641503CC
      "h{lószoba"
};
TempSensor sensor7 = {
      {0x28, 0xFF, 0x83, 0x06, 0x64, 0x15, 0x03, 0x0E},   //28.FF83066415030E
      "el#t|r"
};
TempSensor sensor8 = {
      {0x28, 0xFF, 0xF2, 0xF0, 0x63, 0x15, 0x01, 0x10},   //28.FFF2F063150110
      "k~ls#"
};

TempSensor sensor9 = {
      {0x28, 0xFF, 0x29, 0x39, 0x71, 0x15, 0x01, 0x91},    //28.FF293971150191
      "speiz"
};
TempSensor sensor10 = {
      {0x28, 0xFF, 0x27, 0x42, 0x71, 0x15, 0x01, 0x0F},   //28.FF27427115010F
      "padl{s"
};
TempSensor sensor11 = {
      {0x28, 0xFF, 0x30, 0x7E, 0x71, 0x15, 0x01, 0x78},   //28.FF307E71150178
      "gar{zs"
};
TempSensor sensor12 = {
      {0x28, 0xFF, 0xBB, 0xA3, 0x63, 0x15, 0x01, 0x5A},   //28.FFBBA36315015A
      "pince"
};

#endif

