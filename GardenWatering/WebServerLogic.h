//  WebServerLogic.h - Arduino web server Logic header that serving web interface for scheduler
#pragma once
#ifndef WEB_SERVER_LOGIC_H
#define WEB_SERVER_LOGIC_H

#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include "FS.h"

#include "WifiConfig.h"
#include "SchedulerLogic.h"
//#include "SchedulerWebAdapter.h"
#include "commonFunctions.h"
#include "DS18B20.h"
#include "MCPManagement.h"

#include <Adafruit_BMP085.h>
#include <NtpClientLib.h>

SchedulerLogic scheduler;
//SchedulerWebAdapter schedulerWebAdapter(&scheduler);

ESP8266WebServer server(80);
int32_t WifiConnectionNr = 0;

Adafruit_BMP085 bmp;
float Temperature = 0;
int32_t Pressure = 0;
float Altitude = 0;

DS18B20TempCollection OneWireTempCollection;


MCPManagement momentary;
MCPManagement buttons(1);
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

void handleMCPManual();
void handleMCPManualButtonUpdate();
void handleMCPManualZoneUpdate();

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
    WifiConnectionNr += 1;
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

  server.on("/mcpmanual", handleMCPManual);
  server.on("/mcpmanualbutton_update", HTTP_GET, handleMCPManualButtonUpdate);
  server.on("/mcpmanualzone_update", HTTP_GET, handleMCPManualZoneUpdate);

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
  HTMLMessage += "<br> Wifi network connection nr: ";
  HTMLMessage += String(WifiConnectionNr);

  server.send(200, "text/html", HTMLMessage);
}

void handleTemp() {
  ledON();
  Temperature = bmp.readTemperature();
  Serial.print("Temperature = ");
  Serial.print(Temperature/100);
  Serial.println(" Ă‚Â°C");

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
  HTMLMessage += " meters\n";


  String TempIntakeText = String(OneWireTempCollection.getSensorValue(INTAKE));
  TempIntakeText += "°C";
  String TempPoolText = String(OneWireTempCollection.getSensorValue(POOL));
  TempPoolText += "°C";
  String TempInText = String(OneWireTempCollection.getSensorValue(IN));
  TempInText += "°C";
  String TempOutText = String(OneWireTempCollection.getSensorValue(OUT));
  TempOutText += "°C";

  HTMLMessage += "\n Intake Temperature = ";
  HTMLMessage += TempIntakeText;
  HTMLMessage += "\n Pool Temperature = ";
  HTMLMessage += TempPoolText;
  HTMLMessage += "\n In Temperature = ";
  HTMLMessage += TempInText;
  HTMLMessage += "\n Out Temperature = ";
  HTMLMessage += TempOutText;

  
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
  bool bPoolActive = scheduler.isPoolActive();
  String PoolText = "Pool Pump: ";
  PoolText += "    <div class=\"";
  PoolText += bPoolActive ? "on" : "off";
  PoolText += "\">";
  PoolText += bPoolActive ? "ON" : "OFF";
  PoolText += "    </div>";

  String TempIntakeText = String(OneWireTempCollection.getSensorValue(IN));
  TempIntakeText += "°C";
  String TempPoolText = String(OneWireTempCollection.getSensorValue(POOL));
  TempPoolText += "°C";
  
  String HTMLMessage = "<!DOCTYPE html><html>";
  HTMLMessage += "<head><meta charset=\"UTF-8\">";
  HTMLMessage += "<style>";
  HTMLMessage += "   .container {  position: relative;  text-align: center;  color: black; font-weight: bold; font-size: 150%}";
  HTMLMessage += "   .pump {  position: absolute;  bottom: 8px;  left: 16px;}";
  HTMLMessage += "   .tempIntake {  position: absolute;  top: 50%;  right: 35px;}";
  HTMLMessage += "   .tempPool   {  position: absolute;  top: 25%;  left: 25%; color: white;}";
  HTMLMessage += "   .on   {  color: green;}";
  HTMLMessage += "   .off  {  color: red;}";
  HTMLMessage += "</style></head>";
  HTMLMessage += "<body>";
  HTMLMessage += "System status!\n<p>";
  HTMLMessage += "Zone = ";
  HTMLMessage += scheduler.isZoneActive() ? "ON" : "OFF";
  if (scheduler.isZoneActive()) {
    HTMLMessage += " (Active: ";
    HTMLMessage += scheduler.getActiveZone();
    HTMLMessage += " )";
  }
  HTMLMessage += "\n<p>";
  HTMLMessage += PoolText;
  HTMLMessage += "\n";
  HTMLMessage += "<p><div class=\"container\">";
  HTMLMessage += "    <img src=\"https://static.spacecrafted.com/fe5d391b3ce44129984e44bcd18d549d/i/dfe2fa81e8474d71bdff6fb30e75f960/1/4SoifmQp45JMgBnHp7ed2/Figure-5-Electric-Swimming-Pool-Heater.png\" alt=\"Smiley face\" style=\"width:100%;\">";
  HTMLMessage += "    <div class=\"pump\">";
  HTMLMessage +=        PoolText;
  HTMLMessage += "    </div>";
  HTMLMessage += "    <div class=\"tempIntake\">";
  HTMLMessage +=        TempIntakeText;
  HTMLMessage += "    </div>";
  HTMLMessage += "    <div class=\"tempPool\">";
  HTMLMessage +=        TempPoolText;
  HTMLMessage += "    </div>";
  HTMLMessage += "</div>";
  HTMLMessage += "<p><img src=\"data:Electric-Swimming-Pool-Heater.png\" alt=\"Smiley face\" >";//height=\"42\" width=\"42\"> ";
  HTMLMessage += "</body></html>";

  server.send(200, "text/html", HTMLMessage);
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


/*
 *  MCP manual update
 */

void handleMCPManualUpdate(MCPManagement& MCP) {
  for (uint8_t i = 0; i < server.args(); i++) {
    int port = server.argName(i).toInt();
    int value = server.arg(i).toInt();
    MCP.setOutput(port, value);
    break;
  }
  server.sendHeader("Location","/mcpmanual");
  server.send(303);
}

void handleMCPManualButtonUpdate() {
  handleMCPManualUpdate(buttons);
}


void handleMCPManualMomentaryUpdate(MCPManagement& MCP) {
  for (uint8_t i = 0; i < server.args(); i++) {
    int port = server.argName(i).toInt();
    int value = server.arg(i).toInt();
    MCP.setOutput(port, 0);
    delay(1000);
    MCP.setOutput(port, 1);
    break;
  }
  server.sendHeader("Location","/mcpmanual");
  server.send(303);
}

void handleMCPManualZoneUpdate() {
  handleMCPManualMomentaryUpdate(momentary);
}

String generateTable(MCPManagement& MCP, String action, String title) {
  String HTMLMessage = "<div class=\"center\">\n";
  HTMLMessage += "  <p><b>"+title+"</b></p>\n";
  HTMLMessage += "  <table>\n";
  HTMLMessage += "    <tr>\n";
  HTMLMessage += "      <th>Port</th>\n";
  HTMLMessage += "      <th>Status</th>\n";
  HTMLMessage += "    </tr>\n";
  for( int i=0; i<NR_OF_PORTS; i++) {
    HTMLMessage += "    <tr>\n";
    HTMLMessage += "      <th>" + String(i+1) + "</th>\n";
    HTMLMessage += "      <th>\n";
    HTMLMessage += "          <form method=\"get\" action=\"/"+action+"\">";
    String value = "1";
    String type = "ON";
    if ( MCP.getOutput(i)) {
      value = "0";
      type = "OFF";
    }
    HTMLMessage += "           <button type=\"submit\" name=\""+ String(i)+"\" value=\""+value+"\" class=\"button button"+type+"\"> </button>";
    HTMLMessage += "         </form>\n";
    HTMLMessage += "     </th>\n";
    HTMLMessage += "    </tr>\n";     
  }
  HTMLMessage += "  </table>\n";
  HTMLMessage += "</div>\n";

  return HTMLMessage;
}
void handleMCPManual() {
  String HTMLMessage = "<!DOCTYPE html><html>";
  HTMLMessage += "<head><meta charset=\"UTF-8\">";
  HTMLMessage += "<style>";
  HTMLMessage += "   .center {";
  HTMLMessage += "     margin: auto;";
  HTMLMessage += "     width: 60%;";
  HTMLMessage += "     border: 3px solid #73AD21;";
  HTMLMessage += "     padding: 10px;";
  HTMLMessage += "     text-align: center;";
  HTMLMessage += "   }";
  /*HTMLMessage += "   table, th, td {";
  HTMLMessage += "     border: 1px solid black;";
  HTMLMessage += "   }";*/


  HTMLMessage += "   table {";
  HTMLMessage += "     border-collapse: collapse;";
  HTMLMessage += "     width: 100%;";
  HTMLMessage += "   }";
  
  HTMLMessage += "   th, td {";
  HTMLMessage += "     text-align: center;";
  HTMLMessage += "     padding: 8px;";
  HTMLMessage += "   }";

  HTMLMessage += "   .button {";
  HTMLMessage += "     background-color: #4CAF50; /* Green */";
  HTMLMessage += "     box-shadow: 0 8px 16px 0 rgba(0,0,0,0.2), 0 6px 20px 0 rgba(0,0,0,0.19);";
  HTMLMessage += "     border: none;";
  HTMLMessage += "     color: white;";
  HTMLMessage += "     padding: 16px 32px;";
  HTMLMessage += "     text-align: center;";
  HTMLMessage += "     text-decoration: none;";
  HTMLMessage += "     display: inline-block;";
  HTMLMessage += "     font-size: 16px;";
  HTMLMessage += "     margin: 4px 2px;";
  HTMLMessage += "     transition-duration: 0.4s;";
  HTMLMessage += "     cursor: pointer;";
  HTMLMessage += "   }";

  HTMLMessage += "   .buttonOFF:hover {";
  HTMLMessage += "     background-color: #e7e7e7;";
  HTMLMessage += "   }";

  HTMLMessage += "   .buttonOFF {";
  HTMLMessage += "     background-color: white; ";
  HTMLMessage += "     color: black;";
  HTMLMessage += "     border: 2px solid #4CAF50;";
  HTMLMessage += "   }";

  HTMLMessage += "   .buttonON {";
  HTMLMessage += "     background-color: #4CAF50;";
  HTMLMessage += "     color: white;";
  HTMLMessage += "   }";

  HTMLMessage += "   tr:nth-child(even) {background-color: #f2f2f2;}";
  HTMLMessage += "</style></head>";

  HTMLMessage += "<body>";
  HTMLMessage += generateTable(buttons, "mcpmanualbutton_update", "MCP manual Buttons managenment!");
  HTMLMessage += "  <p>";
  //HTMLMessage += generateTable(momentary, "mcpmanualzone_update", "MCP manual Zones managenment!");
  HTMLMessage += "</body></html>";

  server.send(200, "text/html", HTMLMessage);
}

#endif //WEB_SERVER_LOGIC_H
