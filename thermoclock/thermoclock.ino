// Include the libraries we need
#include <OneWire.h>
#include <DallasTemperature.h>
#include <Wire.h>
#include "SSD1306.h"

#include <ESP8266WiFi.h>
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <WiFiManager.h>

#include <NTPClient.h>

WiFiUDP ntpUDP;

NTPClient timeClient(ntpUDP, "europe.pool.ntp.org", 0, 60000);

// Data wire is plugged into port 2 on the Arduino
#define ONE_WIRE_BUS 2

// Setup a oneWire instance to communicate with any OneWire devices (not just Maxim/Dallas temperature ICs)
OneWire oneWire(ONE_WIRE_BUS);

// Pass our oneWire reference to Dallas Temperature. 
DallasTemperature sensors(&oneWire);

// Initialize the OLED display using Wire library
SSD1306  display(0x3c, D2, D1);

/*
 * The setup function. We only start the sensors here
 */
void setup(void)
{
  // start serial port
  Serial.begin(9600);
  Serial.println("Dallas Temperature IC Control Library Demo");

  //Wifi captive portal
  WiFiManager wifiManager;

  // Start up the library
  sensors.begin();

  display.init();
  display.flipScreenVertically();
  display.setFont(ArialMT_Plain_10);
  display.clear();
  display.drawString(0,0,"WIFI Connecting..");
  display.display();
  wifiManager.autoConnect("Thermoclock");
  display.clear();
  display.drawString(0,0,"WIFI Connected");
  display.display();
  delay(2000);

  WiFi.begin();

  timeClient.begin();
}

/*
 * Main function, get and show the temperature
 */
void loop(void)
{ 
  int hours;
  int minutes;
  
  display.clear();
  // call sensors.requestTemperatures() to issue a global temperature 
  // request to all devices on the bus
  Serial.print("Requesting temperatures...");
  sensors.requestTemperatures(); // Send the command to get temperatures
  Serial.println("DONE");
  // After we got the temperatures, we can print them here.
  // We use the function ByIndex, and as an example get the temperature from the first sensor only.
  Serial.print("Temperature for the device 1 (index 0) is: ");
  Serial.println(sensors.getTempCByIndex(0));
  display.setFont(ArialMT_Plain_10);
  char tempstr[5];
  String displaystr = "Temperature: ";
  dtostrf(sensors.getTempCByIndex(0),5,2,tempstr);
  displaystr.concat(tempstr);
  displaystr.concat("°C");
  
  display.drawString(0, 19, displaystr);
  display.display();

  timeClient.update();
  Serial.println(timeClient.getFormattedTime());

  // display.drawString(0,30, timeClient.getFormattedTime());

  hours = timeClient.getHours();
  minutes = timeClient.getMinutes();
  
  String hoursStr = hours < 10 ? "0" + String(hours) : String(hours);
  String minuteStr = minutes < 10 ? "0" + String(minutes) : String(minutes);

  display.drawString(0,30, hoursStr + ":" + minuteStr);
  
  display.display();
  
  delay(1000);
  
  
}
