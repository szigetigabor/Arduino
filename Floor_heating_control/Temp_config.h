#ifndef TEMP_CONFIG
#define TEMP_CONFIG

#define ROOMS 4
#define ALL_SENSORS 13

#define ALARM_OFFSET sizeof(byte[8])+sizeof(char[20])
#define DELTA_OFFSET ALARM_OFFSET+sizeof(float)
#define NIGHT_OFFSET DELTA_OFFSET+sizeof(float)

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
      {0x28, 0xFF, 0x7C, 0x89, 0x3D, 0x04, 0x00, 0xBB},
      "konyha",
      0.00f,
      0.1f,
      0.00f,
      0
};

TempSensorData sensor1 = {
      {0x28, 0xC0, 0x10, 0x8F, 0x04, 0x00, 0x00, 0x20},
      "|tkez#",
      12.34f,
      0.1f,
      0.00f,
      1
};
TempSensorData sensor2 = {
      {0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0},
      "f$rd#",
      0.00f,
      0.1f,
      0.00f,
      2
};
TempSensorData sensor3 = {
      {0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0},
      "el#szoba",
      0.00f,
      0.1f,
      0.00f,
      3
};

TempSensor sensor4 = {
      {0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0},
      "nappali"
};

TempSensor sensor5 = {
      {0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0},
      "h{lószoba"
};
TempSensor sensor6 = {
      {0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0},
      "el#t|r"
};
TempSensor sensor7 = {
      {0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0},
      "k~ls#"
};
TempSensor sensor8 = {
      {0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0},
      "wc"
};
TempSensor sensor9 = {
      {0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0},
      "speiz"
};
TempSensor sensor10 = {
      {0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0},
      "padl{s"
};
TempSensor sensor11 = {
      {0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0},
      "gar{zs"
};
TempSensor sensor12 = {
      {0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0},
      "pince"
};

#endif
