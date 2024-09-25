//==========================================================================
//
//      cyd-weather-station.ino
//
//      A simple weather station for the ESP32 Cheap Yellow Display.
//      Shows local environment sensor data and weather forecast
//      from OpenWeatherMap API.
//
//      Inspired by:
//      https://github.com/witnessmenow/ESP32-Cheap-Yellow-Display
//
//==========================================================================
//==========================================================================
//
// Author(s):    Simon Haag
// Contributors: Simon Haag
// Date:         2024-09-25
//
//==========================================================================

// Make sure to copy the UserSetup.h file into the TFT_eSPI library.
// The pins are defined in there.

// ----------------------------
// Standard Libraries
// ----------------------------
#include <Arduino.h>
#include <WiFi.h>

// ----------------------------
// Custom Libraries
// ----------------------------
#include "weatherData.h"
#include "tftGUI.h"
#include "debugLog.h"
#include "projectConfig.h"

// ----------------------------
// Additional Libraries - each one of these will need to be installed.
// ----------------------------
#include <TFT_eSPI.h>
// A library for interfacing with LCD displays
//
// Can be installed from the library manager (Search for "TFT_eSPI")
//https://github.com/Bodmer/TFT_eSPI

// ----------------------------

// ----------------------------
// Variables
// ----------------------------
// Change ssid and password for your local WiFi in projectConf.h.
const char* ssid     = WIFI_SSID;
const char* password = WIFI_PW;

// For a final application, check the API call limits per hour/minute to avoid getting blocked/banned
static unsigned long lastTime = 0;
// Timer set to 10 minutes (600000)
//static unsigned long timerDelay = 600000;
// Set timer to 10 seconds (10000)
static unsigned long timerDelay = 10000;

// Status of WIFI connection
static int wifiStatus;

// Data to store local weather data and weather data from web server
static CWeatherData::OnlineData onlineWeatherData;
static CWeatherData::LocalData localWeatherData;

// Create objects
static CDebugLog debugLog;
static SHT2x sht21;
static CWeatherData weatherData(debugLog, sht21);
static TFT_eSPI tft;
static CTftGUI tftGUI(tft);

void setup()
{
  // Setup TFT display
  tftGUI.begin();

  // Connect to WiFi
  WiFi.begin(ssid, password);
  debugLog.print(DEBUG_LVL_INFO, "Connecting to ", 's');
  debugLog.println(DEBUG_LVL_INFO, ssid, 's');
  do
  {
    wifiStatus = WiFi.status();
    delay(500);
    debugLog.print(DEBUG_LVL_INFO, ".", 's');
  } while(wifiStatus != WL_CONNECTED);

  debugLog.println(DEBUG_LVL_INFO, "", 's');
  debugLog.println(DEBUG_LVL_INFO, "WiFi connected", 's');
  debugLog.println(DEBUG_LVL_INFO, "IP address: ", 's');
  debugLog.println(DEBUG_LVL_INFO, (void*)(String(WiFi.localIP()).c_str()), 's');

  // Setup weather services
  weatherData.begin();

  // Get current weather from web server
  weatherData.getOnlineData(&onlineWeatherData);
  // Get current weather from local sensors
  weatherData.getLocalData(&localWeatherData);
}

void loop()
{
  // Send an HTTP GET request
  if ((millis() - lastTime) > timerDelay)
  {
    weatherData.getLocalData(&localWeatherData);

    if(true == weatherData.getOnlineData(&onlineWeatherData))
    {
      lastTime = millis();

      tftGUI.drawTestScreen(&localWeatherData, &onlineWeatherData);
    }
  }
}
