/*******************************************************************
    A simple weather station for the ESP32 Cheap Yellow Display.
    Shows local environment sensor data and weather forecast
    from OpenWeatherMap API.

    Inspired by:
    https://github.com/witnessmenow/ESP32-Cheap-Yellow-Display

 *******************************************************************/

// Make sure to copy the UserSetup.h file into the TFT_eSPI library.
// The pins are defined in there.

// ----------------------------
// Standard Libraries
// ----------------------------
#include <Arduino.h>
#include <WiFi.h>
#include <Wire.h>

// ----------------------------
// Custom Libraries
// ----------------------------
#include "weatherAPI.h"

// ----------------------------
// Additional Libraries - each one of these will need to be installed.
// ----------------------------
#include <TFT_eSPI.h>
// A library for interfacing with LCD displays
//
// Can be installed from the library manager (Search for "TFT_eSPI")
//https://github.com/Bodmer/TFT_eSPI

#include <SHT2x.h>
// A library for I2C SHT2x series temperature and humidity sensor.
//
// Can be installed from the library manager (Search for "SHT2x")

// ----------------------------

#define SHT21_SDA 22
#define SHT21_SCL 27

// Change ssid and password for your local WiFi.
const char* ssid     = "YourSSID"; // Change this to your WiFi SSID
const char* password = "YourPW"; // Change this to your WiFi password

// For a final application, check the API call limits per hour/minute to avoid getting blocked/banned
unsigned long lastTime = 0;
// Timer set to 10 minutes (600000)
//unsigned long timerDelay = 600000;
// Set timer to 10 seconds (10000)
unsigned long timerDelay = 10000;

WeatherData currentWeather;

TFT_eSPI tft = TFT_eSPI();

SHT2x sht;

void setup()
{
  int wifiStatus;
  Serial.begin(115200);

  // Setup SHT21
  Wire.begin(SHT21_SDA, SHT21_SCL);
  sht.begin();

  // Start the TFT display and set it to black
  tft.init();
  tft.setRotation(3); //This is the display in landscape upside down
  tft.setTextWrap(true, true);

  // Clear the screen before writing to it and set default text colors
  tft.fillScreen(TFT_BLACK);
  tft.setTextColor(TFT_WHITE, TFT_BLACK);

  // Connect to WiFi
  WiFi.begin(ssid, password);
  Serial.print("Connecting to ");
  Serial.println(ssid);
  do
  {
    wifiStatus = WiFi.status();
    delay(500);
    Serial.print(".");
  } while(wifiStatus != WL_CONNECTED);

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  if(wifiStatus == WL_CONNECTED)
  {
    tft.drawString("WiFi connected", 0, 10, 1);
  }

  getWeatherInfo(&currentWeather);
}

void loop()
{
  // Send an HTTP GET request
  if ((millis() - lastTime) > timerDelay)
  {
    sht.read();
    float shtTemperature = sht.getTemperature();
    float shtHumidity = sht.getHumidity();

    Serial.println("SHT21 data:");
    Serial.print("Temperature: ");
    Serial.println(shtTemperature);
    Serial.print("Humidity: ");
    Serial.println(shtHumidity);

    if(true == getWeatherInfo(&currentWeather))
    {
      lastTime = millis();

      char printStr[100];

      snprintf(printStr, sizeof(printStr), "Temperature: %.2f C", currentWeather.temperature);
      tft.drawString(printStr, 0, 60, 2);
      snprintf(printStr, sizeof(printStr), "Feels like: %.2f C", currentWeather.feels_like);
      tft.drawString(printStr, 0, 90, 2);
      snprintf(printStr, sizeof(printStr), "Humidity: %d %%", currentWeather.humidity);
      tft.drawString(printStr, 0, 120, 2);
      snprintf(printStr, sizeof(printStr), "Air Pressure: %d hPa", currentWeather.pressure);
      tft.drawString(printStr, 0, 150, 2);
      snprintf(printStr, sizeof(printStr), "Wind Speed: %.2f m/s", currentWeather.wind_speed);
      tft.drawString(printStr, 0, 180, 2);
    }
  }
}
