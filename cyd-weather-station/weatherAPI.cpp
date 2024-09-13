#include <Arduino.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include "weatherAPI.h"

// Your Domain name with URL path or IP address with path
String openWeatherMapApiKey = "APIKey";

// Replace with your country code and city
String city = "City";
String countryCode = "Country";

String jsonBuffer;

String httpGETRequest(const char* serverName)
{
  WiFiClient client;
  HTTPClient http;

  // Your Domain name with URL path or IP address with path
  http.begin(client, serverName);

  // Send HTTP POST request
  int httpResponseCode = http.GET();

  String payload = "{}";

  if (httpResponseCode>0) {
    Serial.print("HTTP Response code: ");
    Serial.println(httpResponseCode);
    payload = http.getString();
  }
  else {
    Serial.print("Error code: ");
    Serial.println(httpResponseCode);
  }
  // Free resources
  http.end();

  return payload;
}

bool getWeatherInfo(WeatherData *weatherInfo)
{
  bool retVal = false;

  if(NULL != weatherInfo)
  {
    // Check WiFi connection status
    if(WiFi.status() == WL_CONNECTED)
    {
      String serverPath = "http://api.openweathermap.org/data/2.5/weather?q=" + city + "," + countryCode + "&APPID=" + openWeatherMapApiKey;

      jsonBuffer = httpGETRequest(serverPath.c_str());
      // Print full JSON buffer for tests
      // Serial.println(jsonBuffer);

      DynamicJsonDocument doc(4096);
      DeserializationError error = deserializeJson(doc, jsonBuffer);

      if (error)
      {
        Serial.print(F("Weather NOW deserializeJson() failed: "));
        Serial.println(error.f_str());
      }
      else
      {
        JsonObject weather = doc["weather"][0];
        weatherInfo->weather_id = weather["id"];
        strcpy(weatherInfo->description, (const char*)weather["description"]);

        JsonObject main = doc["main"];
        weatherInfo->dt = doc["dt"];
        strcpy(weatherInfo->dt_text, "NOW");

        weatherInfo->temperature = main["temp"];
        // Convert Fahrenheit to Celsius
        weatherInfo->temperature = weatherInfo->temperature - 273.15;
        //weatherInfo->temperature = ((weatherInfo->temperature - 32) * 5) / 9;
        weatherInfo->feels_like = main["feels_like"];
        // Convert Fahrenheit to Celsius
        weatherInfo->feels_like = weatherInfo->feels_like - 273.15;
        //weatherInfo->feels_like = ((weatherInfo->feels_like - 32) * 5) / 9;
        weatherInfo->humidity = main["humidity"];
        weatherInfo->pressure = main["pressure"];

        JsonObject wind = doc["wind"];
        weatherInfo->wind_speed = wind["speed"];

        Serial.print("Temperature: ");
        Serial.println(weatherInfo->temperature);
        Serial.print("Pressure: ");
        Serial.println(weatherInfo->pressure);
        Serial.print("Humidity: ");
        Serial.println(weatherInfo->humidity);
        Serial.print("Wind Speed: ");
        Serial.println(weatherInfo->wind_speed);

        retVal = true;
      }
    }
    else
    {
      Serial.println("WiFi Disconnected");
    }
  }

  return retVal;
}
