int rooms  = 4;
int all_sensors = 8;

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
      "étkező",
      0.00f,
      0.1f,
      0.00f,
      0
};
TempSensorData sensor2 = {
      {0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0},
      "fürdő",
      0.00f,
      0.1f,
      0.00f,
      0
};
TempSensorData sensor3 = {
      {0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0},
      "előszoba",
      0.00f,
      0.1f,
      0.00f,
      0
};

TempSensor sensor4 = {
      {0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0},
      "nappali"
};

TempSensor sensor5 = {
      {0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0},
      "hálószoba"
};
TempSensor sensor6 = {
      {0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0},
      "előtér"
};
TempSensor sensor7 = {
      {0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0},
      "külső"
};
