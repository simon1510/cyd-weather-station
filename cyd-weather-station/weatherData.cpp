#include <Arduino.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include "weatherData.h"


CWeatherData::CWeatherData(const CDebugLog &log, const SHT2x &sht2x) : debugLog(log), sht21(sht2x)
{
  // Nothing to do
}

void CWeatherData::begin(void)
{
  // Setup SHT21
  Wire.begin(SHT21_SDA, SHT21_SCL);
  CWeatherData::sht21.begin();
}

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

        debugLog.print(DEBUG_LVL_INFO, "Temperature: ", 's');
        debugLog.println(DEBUG_LVL_INFO, (void*)&weatherData->temperature, 'f');
        debugLog.print(DEBUG_LVL_INFO, "Pressure: ", 's');
        debugLog.println(DEBUG_LVL_INFO, (void*)&weatherData->pressure, 'i');
        debugLog.print(DEBUG_LVL_INFO, "Humidity: ", 's');
        debugLog.println(DEBUG_LVL_INFO, (void*)&weatherData->humidity, 'f');
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

bool CWeatherData::getLocalData(CWeatherData::LocalData *weatherData)
{
  CWeatherData::sht21.read();
  weatherData->temperature = CWeatherData::sht21.getTemperature();
  weatherData->humidity = CWeatherData::sht21.getHumidity();

  debugLog.println(DEBUG_LVL_INFO, "SHT21 data:", 's');
  debugLog.print(DEBUG_LVL_INFO, "Temperature: ", 's');
  debugLog.println(DEBUG_LVL_INFO, (void*)&weatherData->temperature, 'f');
  debugLog.print(DEBUG_LVL_INFO, "Humidity: ", 's');
  debugLog.println(DEBUG_LVL_INFO, (void*)&weatherData->humidity, 'f');

  return true;
}
