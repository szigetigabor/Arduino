//  WebServerLogic.h - Arduino web server Logic header that serving web interface for scheduler
#pragma once
#ifndef WEB_SERVER_LOGIC_H
#define WEB_SERVER_LOGIC_H

#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>

#include "WifiConfig.h"
#include "SchedulerLogic.h"
//#include "SchedulerWebAdapter.h"
#include "commonFunctions.h"

#include <Adafruit_BMP085.h>
#include <NtpClientLib.h>

SchedulerLogic scheduler;
//SchedulerWebAdapter schedulerWebAdapter(&scheduler);

ESP8266WebServer server(80);

Adafruit_BMP085 bmp;
float Temperature = 0;
int32_t Pressure = 0;
float Altitude = 0;

//const int led = BUILTIN_LED; //13;

// function prototypes for HTTP handlers
void handleRoot();
void handleNotFound();
void handleOFF();
void handleON();
void handleNTPClient();
void handleTemp();
void printDB();
void handleSIGNAL();
void handleSTATUS();
void handleScheduler();
void handleUpdateStartTime();
void handleZoneEnable();
void handleZoneDisable();
void handlePoolScheduler();
void handlePoolUpdateStartTime();
void handlePoolEnable();
void handlePoolDisable();

void WifiInit(int maxRetry = WIFI_CONNECTION_RETRY_COUNT) {
  Serial.println("");
  Serial.print("Connecttion to ");
  Serial.print(WIFI_SSID);
  Serial.println(" network is started...");
  WiFi.begin(WIFI_SSID, WIFI_PASSWD);

  // Wait for connection
  int retry = 0;
  while (WiFi.status() != WL_CONNECTED) {
    if (retry == maxRetry) {
      Serial.println("");
      Serial.print("Could not connect to the ");
      Serial.print(WIFI_SSID);
      Serial.println(" network.");
      break;
    }
    digitalWrite(led, 1);  //ledOFF();
    delay(200);
    digitalWrite(led, 0);  //ledON();
    delay(300);
    Serial.print(".");
    retry++;
  }
  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("");
    Serial.print("Connected to ");
    Serial.println(WIFI_SSID);
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());
  }
}

void webServerInit() {
  if (MDNS.begin("esp8266")) {
    Serial.println("MDNS responder started");
  }

  server.on("/", handleRoot );
  server.on("/temp", handleTemp);
  server.on("/on", handleON);
  server.on("/off", handleOFF);
  //server.on("/ntp", handleNTP);
  server.on("/ntpclient", handleNTPClient);
  
  server.on("/signal", handleSIGNAL);
  server.on("/status", handleSTATUS);
  server.on("/scheduler", HTTP_GET, handleScheduler);
  server.on("/scheduler", HTTP_POST, handleUpdateStartTime);
  server.on("/scheduler_enable", handleZoneEnable);
  server.on("/scheduler_disable", handleZoneDisable);

  server.on("/poolscheduler", HTTP_GET, handlePoolScheduler);
  server.on("/poolscheduler", HTTP_POST, handlePoolUpdateStartTime);
  server.on("/poolscheduler_enable", handlePoolEnable);
  server.on("/poolscheduler_disable", handlePoolDisable);


  server.on("/inline", []() {
    server.send(200, "text/plain", "this works as well");
  });

  server.onNotFound(handleNotFound);

  server.begin();
  Serial.println("HTTP server started");
}

void WifiConnectionCheck() {
  if (WiFi.status() != WL_CONNECTED) {
    WifiInit();
  }
}

///////////////////////////////////
//     http handler funstions    //
///////////////////////////////////
String generateButton(String text, String link) {
  String message;
  message += "<form><input class=\"MyButton\" type=\"button\" value=\"";
  message += text;
  message += "\" onclick=\"window.location.href='";
  message += link;
  message += "'\" /></form>";

  return message;
}

void handleRoot() {
  digitalWrite(led, 1);
  String HTMLMessage = "hello from esp8266!\n";
  HTMLMessage += generateButton("Temperature", "/temp");
  HTMLMessage += generateButton("NTP", "/ntpclient");
  HTMLMessage += generateButton("Wifi signal level", "/signal");
  HTMLMessage += generateButton("Zone Scheduling", "/scheduler");
  HTMLMessage += generateButton("Pool Scheduling", "/poolscheduler");
  HTMLMessage += generateButton("Status", "/status");
  server.send(200, "text/html", HTMLMessage);
  delay(500);
  digitalWrite(led, 0);
}

void handleNotFound() {
  digitalWrite(led, 1);
  String message = "Page Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET) ? "GET" : "POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";
  for (uint8_t i = 0; i < server.args(); i++) {
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }
  server.send(404, "text/plain", message);
  digitalWrite(led, 0);
}

void handleOFF() {
  ledOFF();
  server.send(200, "text/plain", "LED OFF");
}

void handleON() {
  ledON();
  server.send(200, "text/plain", "LED ON");
}

/*void handleNTP(){
  unsigned long epoch = getNTPTime();

  String HTMLMessage = "NTP!\n";
  // print the hour, minute and second:
  HTMLMessage += "The UTC time is ";       // UTC is the time at Greenwich Meridian (GMT)
  HTMLMessage += (epoch  % 86400L) / 3600; // print the hour (86400 equals secs per day)
  HTMLMessage += ':';
  if ( ((epoch % 3600) / 60) < 10 ) {
    // In the first 10 minutes of each hour, we'll want a leading '0'
    HTMLMessage += '0';
  }
  HTMLMessage += (epoch  % 3600) / 60; // print the minute (3600 equals secs per minute)
  HTMLMessage += ':';
  if ( (epoch % 60) < 10 ) {
    // In the first 10 seconds of each minute, we'll want a leading '0'
    HTMLMessage += '0';
  }
  HTMLMessage += epoch % 60; // print the second

  server.send(200, "text/plain", HTMLMessage);
  delay(10000);
  }*/

void handleNTPClient() {

  Serial.print (NTP.getTimeDateString ()); Serial.print (" ");
  Serial.print (NTP.isSummerTime () ? "Summer Time. " : "Winter Time. ");
  Serial.print ("WiFi is ");
  Serial.print (WiFi.isConnected () ? "connected" : "not connected"); Serial.print (". ");
  Serial.print ("Uptime: ");
  Serial.print (NTP.getUptimeString ()); Serial.print (" since ");
  Serial.println (NTP.getTimeDateString (NTP.getFirstSync ()).c_str ());

  String HTMLMessage = "NTPClient!\n";

  HTMLMessage += "NTP configuration!<br>";
  HTMLMessage += NTP.getTimeStr();
  HTMLMessage += "<br>";
  HTMLMessage += NTP.getDateStr();
  HTMLMessage += "<br>";
  HTMLMessage += NTP.getTimeDateString();
  HTMLMessage += " ";
  HTMLMessage += NTP.isSummerTime() ? "Summer Time. " : "Winter Time. ";
  HTMLMessage += "<br>Last sync time: ";
  HTMLMessage += NTP.getTimeDateString(NTP.getLastNTPSync());
  HTMLMessage += "<br>Uptime: ";
  HTMLMessage += NTP.getUptimeString();
  HTMLMessage += " since ";
  HTMLMessage += NTP.getTimeDateString(NTP.getFirstSync()).c_str();

  server.send(200, "text/html", HTMLMessage);
}

void handleTemp() {
  ledON();
  Temperature = bmp.readTemperature();
  Serial.print("Temperature = ");
  Serial.print(Temperature/100);
  Serial.println(" °C");

  Pressure = bmp.readPressure();
  Serial.print("Pressure = ");
  Serial.print(Pressure);
  Serial.println(" Pa");

  // Calculate altitude assuming 'standard' barometric
  // pressure of 1013.25 millibar = 101325 Pascal
  Altitude = bmp.readAltitude();
  Serial.print("Altitude = ");
  Serial.print(Altitude);
  Serial.println(" meters");

  Serial.print("Pressure at sea level (calculated) = ");
  Serial.print(bmp.readSealevelPressure());
  Serial.println(" Pa");

  // you can get a more precise measurement of altitude
  // if you know the current sea level pressure which will
  // vary with weather and such. If it is 1015 millibars
  // that is equal to 101500 Pascals.
  Serial.print("Real altitude = ");
  Serial.print(bmp.readAltitude(101500));
  Serial.println(" meters");

  Serial.println();
  String HTMLMessage = "hello from esp8266 Temperature!\n";
  HTMLMessage += "Temperature = ";
  HTMLMessage += Temperature;
  HTMLMessage += "°C\n";
  HTMLMessage += "Pressure = ";
  HTMLMessage += Pressure;
  HTMLMessage += " Pa\n";
  HTMLMessage += "Altitude = ";
  HTMLMessage += Altitude;
  HTMLMessage += " meters";

  server.send(200, "text/plain", HTMLMessage);
  //delay(500);
  ledOFF();
}

void printDB() {
  Serial.print(WiFi.RSSI());
  Serial.println(" dB");
  unsigned long start = millis();
  do {
    //serviceAlarms();
    yield();
  } while (millis() - start  <= 1000 /*ms*/);
}

void handleSIGNAL() {
  long rssi;
  rssi = WiFi.RSSI(); // eg. -63

  String HTMLMessage = "WIFI signal level!\n";
  HTMLMessage += "SSID = ";
  HTMLMessage += WIFI_SSID;
  HTMLMessage += "\nlevel = ";
  HTMLMessage += rssi;
  HTMLMessage += " dB\n";

  server.send(200, "text/plain", HTMLMessage);
}

void handleSTATUS() {
  String HTMLMessage = "System status!\n";
  HTMLMessage += "Zone = ";
  HTMLMessage += scheduler.isZoneActive() ? "ON" : "OFF";
  if (scheduler.isZoneActive()) {
    HTMLMessage += " (Active: ";
    HTMLMessage += scheduler.getActiveZone();
    HTMLMessage += " )";
  }
  HTMLMessage += "\nPool = ";
  HTMLMessage += scheduler.isPoolActive() ? "ON" : "OFF";
  HTMLMessage += "\n";

  server.send(200, "text/plain", HTMLMessage);
}

void handleSchedulerTemplate(bool pool = false) {
  String HTMLMessage = "Zones scheduler configuration!<br>";
  // scheduler enable/disable
  bool enabled = scheduler.isAlarmsEnabled();
  String prefix = "";
  if (pool) {
    enabled = scheduler.isPoolAlarmsEnabled();
    prefix = "pool";
  }
  HTMLMessage += "<form action=\"/"+prefix+"scheduler_enable\" method=\"POST\">";
  HTMLMessage += "<input type=\"submit\" value=\"Enable\"";
  if (enabled) HTMLMessage += "disabled";
  HTMLMessage += "></form>";
  HTMLMessage += "<form action=\"/"+prefix+"scheduler_disable\" method=\"POST\">";
  HTMLMessage += "<input type=\"submit\" value=\"Disable\"";
  if (!enabled) HTMLMessage += "disabled";
  HTMLMessage += "></form>";
  
  // Start time set
  HTMLMessage += "Start time!<br>";
  HTMLMessage += "<form action=\"/"+prefix+"scheduler\" method=\"POST\">";
  if (pool){
    HTMLMessage += "<input type=\"text\" name=\"hour\" value=\"" + String(scheduler.getPoolStartHour()) + "\" maxlength=\"2\" size=\"2\">";
    HTMLMessage += ":";
    HTMLMessage += "<input type=\"text\" name=\"minute\" value=\"" + String(scheduler.getPoolStartMinute()) + "\" maxlength=\"2\" size=\"2\">";
  } else {
    HTMLMessage += "<input type=\"text\" name=\"hour\" value=\"" + String(scheduler.getStartHour()) + "\" maxlength=\"2\" size=\"2\">";
    HTMLMessage += ":";
    HTMLMessage += "<input type=\"text\" name=\"minute\" value=\"" + String(scheduler.getStartMinute()) + "\" maxlength=\"2\" size=\"2\">";
  }
  HTMLMessage += "<input type=\"submit\" value=\"Update Start Time\"></form>";
  server.send(200, "text/html", HTMLMessage);
}

void handleScheduler() {
  handleSchedulerTemplate();
}

void handlePoolScheduler() {
  handleSchedulerTemplate(true);
}

void handleUpdateStartTimeTemplate(bool pool = false) { // If a POST request is made to URI /scheduler
  if( ! server.hasArg("hour") || ! server.hasArg("minute") 
      || server.arg("hour") == NULL || server.arg("minute") == NULL) { // If the POST request doesn't have hour and minute data
    server.send(400, "text/plain", "400: Invalid Request");         // The request is invalid, so send HTTP status 400
    return;
  }
  if (pool) {
    scheduler.setPoolStartHour(server.arg("hour").toInt());
    scheduler.setPoolStartMinute(server.arg("minute").toInt());
    server.sendHeader("Location","/poolscheduler");
    
  } else {
    scheduler.setStartHour(server.arg("hour").toInt());
    scheduler.setStartMinute(server.arg("minute").toInt());
    server.sendHeader("Location","/scheduler");
  }
  server.send(303);
}

void handleUpdateStartTime() {
  handleUpdateStartTimeTemplate();
}

void handlePoolUpdateStartTime() {
  handleUpdateStartTimeTemplate(true);
}

void handleZoneEnable() {
  scheduler.EnableZoneAlarms();
  server.sendHeader("Location","/scheduler");
  server.send(303);
}

void handleZoneDisable() {
  scheduler.DisableZoneAlarms();
  server.sendHeader("Location","/scheduler");
  server.send(303);
}

void handlePoolEnable() {
  scheduler.EnablePoolAlarms();
  server.sendHeader("Location","/poolscheduler");
  server.send(303);
}

void handlePoolDisable() {
  scheduler.DisablePoolAlarms();
  server.sendHeader("Location","/poolscheduler");
  server.send(303);
}

#endif //WEB_SERVER_LOGIC_H
