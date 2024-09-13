#ifndef WEATHERDATA_H
#define WEATHERDATA_H

#include "debugLog.h"
#include "projectConfig.h"

// ----------------------------
// Additional Libraries - each one of these will need to be installed.
// ----------------------------
#include <SHT2x.h>
// A library for I2C SHT2x series temperature and humidity sensor.
//
// Can be installed from the library manager (Search for "SHT2x")

class CWeatherData
{
  public:

    // Data fetched from online weather services
    struct OnlineData
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

    // Data fetched from local environment sensors
    struct LocalData
    {
      float temperature;
      float humidity;
    };

    CWeatherData(const CDebugLog &log, const SHT2x &sht2x);
    void begin(void);
    bool getOnlineData(OnlineData *weatherData);
    bool getLocalData(LocalData *weatherData);

  private:
    // API Key, country code and city from projectConfig.h
    String openWeatherMapApiKey = OPENWEATHERMAP_API_KEY;
    String city = OPENWEATHERMAP_CITY;
    String countryCode = OPENWEATHERMAP_COUNTRY;

    String jsonBuffer;

    // Local objects
    CDebugLog debugLog;
    SHT2x sht21;

    String httpGETRequest(const char* serverName);
};

#endif // WEATHERDATA_H
