//==========================================================================
//
//      weatherData.cpp
//
//      A module which fetches weather and environmental data
//      from a web server and from local sensors.
//
//==========================================================================
//==========================================================================
//
// Author(s):    Simon Haag
// Contributors: Simon Haag
// Date:         2024-09-25
//
//==========================================================================

// ----------------------------
// Standard Libraries
// ----------------------------
#include <Arduino.h>
#include <WiFi.h>

// ----------------------------
// Custom Libraries
// ----------------------------
#include "weatherData.h"

// ----------------------------
// Additional Libraries - each one of these will need to be installed.
// ----------------------------
#include <ArduinoJson.h>
// A library for handling JSON formats
//
// Can be installed from the library manager (Search for "ArduinoJson")

#include <HTTPClient.h>
// A library for making HTTP GET, POST and PUT requests to a web server
//
// Can be installed from the library manager (Search for "HttpClient")

//***********************************************
//  CWeatherData::CWeatherData()
//***********************************************
CWeatherData::CWeatherData(const CDebugLog &log, const SHT2x &sht2x) : debugLog(log), sht21(sht2x)
{
  // Nothing to do
}

//***********************************************
//  CWeatherData::begin()
//***********************************************
void CWeatherData::begin(void)
{
  // Setup SHT21
  Wire.begin(SHT21_SDA, SHT21_SCL);
  CWeatherData::sht21.begin();
}

//***********************************************
//  CWeatherData::httpGETRequest()
//***********************************************
String CWeatherData::httpGETRequest(const char* serverName)
{
  WiFiClient client;
  HTTPClient http;

  // Your Domain name with URL path or IP address with path
  http.begin(client, serverName);

  // Send HTTP POST request
  int httpResponseCode = http.GET();

  String payload = "{}";

  if (httpResponseCode>0) {
    debugLog.print(DEBUG_LVL_ERROR, "HTTP Response code: ", 's');
    debugLog.println(DEBUG_LVL_ERROR, (void*)&httpResponseCode, 'i');
    payload = http.getString();
  }
  else {
    debugLog.print(DEBUG_LVL_INFO, "Error code: ", 's');
    debugLog.println(DEBUG_LVL_INFO, (void*)&httpResponseCode, 'i');
  }
  // Free resources
  http.end();

  return payload;
}

//***********************************************
//  CWeatherData::getOnlineData()
//***********************************************
bool CWeatherData::getOnlineData(CWeatherData::OnlineData *weatherData)
{
  bool retVal = false;

  if(NULL != weatherData)
  {
    // Check WiFi connection status
    if(WiFi.status() == WL_CONNECTED)
    {
      String serverPath = "http://api.openweathermap.org/data/2.5/weather?q=" + city + "," + countryCode + "&APPID=" + openWeatherMapApiKey;

      jsonBuffer = httpGETRequest(serverPath.c_str());
      // Print full JSON buffer for tests
      // debugLog.println(DEBUG_LVL_INFO, jsonBuffer, 's);

      DynamicJsonDocument doc(4096);
      DeserializationError error = deserializeJson(doc, jsonBuffer);

      if (error)
      {
        debugLog.print(DEBUG_LVL_ERROR, F("Weather NOW deserializeJson() failed: "), 's');
        debugLog.println(DEBUG_LVL_ERROR, error.f_str(), 's');
      }
      else
      {
        /***********************************************************
         * Parse weather JSON object and store all data into struct
         ***********************************************************/

        JsonObject weather = doc["weather"][0];
        weatherData->weather_id = weather["id"];
        strcpy(weatherData->description, (const char*)weather["description"]);

        JsonObject main = doc["main"];
        weatherData->dt = doc["dt"];
        strcpy(weatherData->dt_text, "NOW");

        weatherData->temperature = main["temp"];
        // Convert Kelvin to Celsius
        weatherData->temperature = weatherData->temperature - 273.15;
        weatherData->feels_like = main["feels_like"];
        // Convert Kelvin to Celsius
        weatherData->feels_like = weatherData->feels_like - 273.15;
        weatherData->humidity = main["humidity"];
        weatherData->pressure = main["pressure"];

        JsonObject wind = doc["wind"];
        weatherData->wind_speed = wind["speed"];

        // Print weather data from web server to console
        debugLog.print(DEBUG_LVL_INFO, "Temperature: ", 's');
        debugLog.println(DEBUG_LVL_INFO, (void*)&weatherData->temperature, 'f');
        debugLog.print(DEBUG_LVL_INFO, "Pressure: ", 's');
        debugLog.println(DEBUG_LVL_INFO, (void*)&weatherData->pressure, 'i');
        debugLog.print(DEBUG_LVL_INFO, "Humidity: ", 's');
        debugLog.println(DEBUG_LVL_INFO, (void*)&weatherData->humidity, 'i');
        debugLog.print(DEBUG_LVL_INFO, "Wind Speed: ", 's');
        debugLog.println(DEBUG_LVL_INFO, (void*)&weatherData->wind_speed, 'f');

        retVal = true;
      }
    }
    else
    {
      debugLog.println(DEBUG_LVL_ERROR, "WiFi Disconnected", 's');
    }
  }

  return retVal;
}

//***********************************************
//  CWeatherData::getLocalData()
//***********************************************
bool CWeatherData::getLocalData(CWeatherData::LocalData *weatherData)
{
  // Read data from SHT21 sensor
  CWeatherData::sht21.read();
  weatherData->temperature = CWeatherData::sht21.getTemperature();
  weatherData->humidity = CWeatherData::sht21.getHumidity();

  // Print SHT21 sensor data to console
  debugLog.println(DEBUG_LVL_INFO, "SHT21 data:", 's');
  debugLog.print(DEBUG_LVL_INFO, "Temperature: ", 's');
  debugLog.println(DEBUG_LVL_INFO, (void*)&weatherData->temperature, 'f');
  debugLog.print(DEBUG_LVL_INFO, "Humidity: ", 's');
  debugLog.println(DEBUG_LVL_INFO, (void*)&weatherData->humidity, 'f');

  return true;
}
