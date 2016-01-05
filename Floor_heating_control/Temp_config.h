int rooms  = 4;

struct TempSensorData {
  byte addr[8];
  float alarm;
  float delta;
  float night;
  char name[20];
};

TempSensorData sensor0 = {
      {0x28, 0xFF, 0x7C, 0x89, 0x3D, 0x04, 0x00, 0xBB},
      0.00f,
      0.1f,
      0.00f,
      "konyha"
};

TempSensorData sensor1 = {
      {0x28, 0xC0, 0x10, 0x8F, 0x04, 0x00, 0x00, 0x20},
      0.00f,
      0.1f,
      0.00f,
      "etkezo"
};
TempSensorData sensor2 = {
      {0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0},
      0.00f,
      0.1f,
      0.00f,
      "furdo"
};
TempSensorData sensor3 = {
      {0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0},
      0.00f,
      0.1f,
      0.00f,
      "eloszoba"
};


