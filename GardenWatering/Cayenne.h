#define CAYENNE_PRINT Serial
#include <CayenneMQTTESP8266.h>
#include <SimpleTimer.h>    //http://playground.arduino.cc/Code/SimpleTimer      https://github.com/jfturcot/SimpleTimer

#include "CayenneConfig.h"
#include "WifiConfig.h"
#include "DS18B20.h"
DS18B20TempCollection OneWireTemperatureCollection;


#define ZONE_1_CHANNEL 1
#define ZONE_2_CHANNEL 2
#define ZONE_3_CHANNEL 3
#define ZONE_4_CHANNEL 4
#define ZONE_5_CHANNEL 5
#define ZONE_6_CHANNEL 6

#define POOL_LIGHT_CHANNEL     7
#define POOL_CHANNEL           8  
#define BATTERY_CHARGE_CHANNEL 9
#define BATTERY_CHARGE_ENABLE_CHANNEL 10

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

  
}

static void sensors()
{
  Cayenne.virtualWrite(5, getBatteryVoltage(), "Battery", "V"); 
  Cayenne.virtualWrite(6, (int)getBatteryPercentage(), "Battery percent", "%");

  Cayenne.virtualWrite(7, (int)enableZonesFromSensor(), "RainSensor", "b");
}

static void keepAlive()
{
  // This function is called at intervals to send data to Cayenne and keep the device online.
  Cayenne.virtualWrite(10, millis() / 1000);
}
void CayenneInit() {
  Cayenne.begin(CAYENNE_MQTT_USERNAME, CAYENNE_MQTT_PASSWORD, CAYENNE_CLIENT_ID, WIFI_SSID, WIFI_PASSWD);

  // Set up a function to be called every 2 minutes
  long interval = CAYENNE_DATA_SENDING_FREQUENCY * 1000L;
  timer.setInterval(interval, valueRegistration);

  // Set up a function to be called every 11 seconds
  timer.setInterval(interval+2, sensors);
  
  // Set up a function to be called every 5 seconds
  timer.setInterval(5000L, keepAlive);
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

void switchMomentaryButton(int port, int value)
{
  MCPMomentaryManagement* MCP = getMomentaryButtonPtr();
  if (value == 0) {
    ledOFF();
    MCP->setOutput(port, HIGH);  // turn OFF
  } else {
    ledON();
    MCP->setOutput(port, LOW);  // turn ON
  }  
}

void switchButton(int port, int value)
{
  MCPManagement* MCP = getButtonPtr();
  if (value == 0) {
    ledOFF();
    MCP->setOutput(port, HIGH);  // turn OFF
  } else {
    ledON();
    MCP->setOutput(port, LOW);  // turn ON
  }  
}


// Default function for processing actuator commands from the Cayenne Dashboard.
// You can also use functions for specific channels, e.g CAYENNE_IN(1) for channel 1 commands.
//CAYENNE_IN_DEFAULT()
CAYENNE_IN(ZONE_1_CHANNEL)
{
  CAYENNE_LOG("Channel %u, value %s", request.channel, getValue.asString());
  //Process message here. If there is an error set an error message using getValue.setError(), e.g getValue.setError("Error message");
  switchMomentaryButton(ZONE_1_CHANNEL-1,  getValue.asInt());
}

CAYENNE_IN(ZONE_2_CHANNEL)
{
  CAYENNE_LOG("Channel %u, value %s", request.channel, getValue.asString());
  //Process message here. If there is an error set an error message using getValue.setError(), e.g getValue.setError("Error message");
  switchMomentaryButton(ZONE_2_CHANNEL-1,  getValue.asInt());
}

CAYENNE_IN(ZONE_3_CHANNEL)
{
  CAYENNE_LOG("Channel %u, value %s", request.channel, getValue.asString());
  //Process message here. If there is an error set an error message using getValue.setError(), e.g getValue.setError("Error message");
  switchMomentaryButton(ZONE_3_CHANNEL-1,  getValue.asInt());
}

CAYENNE_IN(ZONE_4_CHANNEL)
{
  CAYENNE_LOG("Channel %u, value %s", request.channel, getValue.asString());
  //Process message here. If there is an error set an error message using getValue.setError(), e.g getValue.setError("Error message");
  switchMomentaryButton(ZONE_4_CHANNEL-1,  getValue.asInt());
}

CAYENNE_IN(ZONE_5_CHANNEL)
{
  CAYENNE_LOG("Channel %u, value %s", request.channel, getValue.asString());
  //Process message here. If there is an error set an error message using getValue.setError(), e.g getValue.setError("Error message");
  switchMomentaryButton(ZONE_5_CHANNEL-1,  getValue.asInt());
}

CAYENNE_IN(ZONE_6_CHANNEL)
{
  CAYENNE_LOG("Channel %u, value %s", request.channel, getValue.asString());
  //Process message here. If there is an error set an error message using getValue.setError(), e.g getValue.setError("Error message");
  switchMomentaryButton(ZONE_6_CHANNEL-1,  getValue.asInt());
}

CAYENNE_IN(POOL_LIGHT_CHANNEL)
{
  CAYENNE_LOG("Channel %u, value %s", request.channel, getValue.asString());
  //Process message here. If there is an error set an error message using getValue.setError(), e.g getValue.setError("Error message");
  switchMomentaryButton(POOL_LIGHT_CHANNEL-1,  getValue.asInt());
}

CAYENNE_IN(POOL_CHANNEL)
{
  CAYENNE_LOG("Channel %u, value %s", request.channel, getValue.asString());
  //Process message here. If there is an error set an error message using getValue.setError(), e.g getValue.setError("Error message");
  switchMomentaryButton(POOL_CHANNEL-1,  getValue.asInt());
}

CAYENNE_IN(BATTERY_CHARGE_CHANNEL)
{
  CAYENNE_LOG("Channel %u, value %s", request.channel, getValue.asString());
  //Process message here. If there is an error set an error message using getValue.setError(), e.g getValue.setError("Error message");
  switchButton(7,  getValue.asInt());
}

CAYENNE_IN(BATTERY_CHARGE_ENABLE_CHANNEL)
{
  CAYENNE_LOG("Channel %u, value %s", request.channel, getValue.asString());
  //Process message here. If there is an error set an error message using getValue.setError(), e.g getValue.setError("Error message");
  if (getValue.asInt() == 0) {
    BATTERY_CHARGING_OFF_ENABLE = false;
  } else {
    BATTERY_CHARGING_OFF_ENABLE = true;
  }  
}
