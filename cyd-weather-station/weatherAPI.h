#ifndef WEATHERAPI_H
#define WEATHERAPI_H

// ----------------------------
// Standard Libraries
// ----------------------------
//#include <Arduino.h>

// ----------------------------
// Additional Libraries - each one of these will need to be installed.
// ----------------------------
//#include <Arduino_JSON.h>
// Can be installed from the library manager (Search for "Arduino_JSON")

struct WeatherData
{
  long dt;
  char dt_text[20];
  float temperature;
  float feels_like;
  int humidity;
  int pressure;
  float wind_speed;
  int weather_id;
  char description[41];
};

bool getWeatherInfo(WeatherData *weatherInfo);

#endif // WEATHERAPI_H
