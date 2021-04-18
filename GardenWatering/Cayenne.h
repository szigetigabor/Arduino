#define CAYENNE_PRINT Serial
#include <CayenneMQTTESP8266.h>
#include <SimpleTimer.h>    //http://playground.arduino.cc/Code/SimpleTimer      https://github.com/jfturcot/SimpleTimer

#include "CayenneConfig.h"
#include "WifiConfig.h"
#include "DS18B20.h"
DS18B20TempCollection OneWireTemperatureCollection;

// Enterprise WiFi network info.
//const char* ssid = "ssid";
//char anonymousIdentity[] = "anonymous@example.com";
//char identity[] = "nickname@example.com";
//char wifiPassword[] = "wifiPassword";


SimpleTimer timer;

// This function sends the device's value to Cayenne on each Virtual Channel.
static void valueRegistration()
{
  // Write data to Cayenne here.
  long rssi = WiFi.RSSI(); // eg. -63
  Cayenne.virtualWrite(0, rssi, "Wifi signal", "dB");

  Cayenne.celsiusWrite(1, OneWireTemperatureCollection.getSensorValue(INTAKE));
  Cayenne.celsiusWrite(2, OneWireTemperatureCollection.getSensorValue(POOL));
  Cayenne.celsiusWrite(3, OneWireTemperatureCollection.getSensorValue(IN));
  Cayenne.celsiusWrite(4, OneWireTemperatureCollection.getSensorValue(OUT));

  Cayenne.virtualWrite(5, getBatteryVoltage(), "Battery", "V");
}


void CayenneInit() {
  Cayenne.begin(CAYENNE_MQTT_USERNAME, CAYENNE_MQTT_PASSWORD, CAYENNE_CLIENT_ID, WIFI_SSID, WIFI_PASSWD);

  // Set up a function to be called every 2 minutes
  long interval = CAYENNE_DATA_SENDING_FREQUENCY * 1000L;
  timer.setInterval(interval, valueRegistration);
}

void CayenneRun() {
  Cayenne.loop(); // Runs main loop
  timer.run(); // Initiates SimpleTimer
}


// Default function for sending sensor data at intervals to Cayenne.
// You can also use functions for specific channels, e.g CAYENNE_OUT(1) for sending channel 1 data.
CAYENNE_OUT_DEFAULT()
{

}

// Default function for processing actuator commands from the Cayenne Dashboard.
// You can also use functions for specific channels, e.g CAYENNE_IN(1) for channel 1 commands.
CAYENNE_IN_DEFAULT()
{
  CAYENNE_LOG("Channel %u, value %s", request.channel, getValue.asString());
  //Process message here. If there is an error set an error message using getValue.setError(), e.g getValue.setError("Error message");
}
