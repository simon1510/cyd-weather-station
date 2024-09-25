#ifndef WEATHERDATA_H
#define WEATHERDATA_H

//==========================================================================
//
//      weatherData.h
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
// Custom Libraries
// ----------------------------
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

    /**************************************************************************
     * Standard constructor
     *
     * Inputs:  log   - Instance of CDebugLog class
     *          sht2x - Instance of SHT2x class
     **************************************************************************/
    CWeatherData(const CDebugLog &log, const SHT2x &sht2x);

    /**************************************************************************
     * Sets up local weather sensors
     **************************************************************************/
    void begin(void);

    /**************************************************************************
     * Fetches weather data from web server.
     * WiFi connection must be established before calling this method.
     *
     * Inputs:  weatherData - Pointer to where the weather data should be stored
     * Output:  bool - Returns true if function call was successful
     **************************************************************************/
    bool getOnlineData(OnlineData *weatherData);

    /**************************************************************************
     * Fetches weather data from local sensors.
     * begin() must be called before calling this method.
     *
     * Inputs:  weatherData - Pointer to where the weather data should be stored
     * Output:  bool - Returns true if function call was successful
     **************************************************************************/
    bool getLocalData(LocalData *weatherData);

  private:
    // API Key, country code and city from projectConfig.h
    String openWeatherMapApiKey = OPENWEATHERMAP_API_KEY;
    String city = OPENWEATHERMAP_CITY;
    String countryCode = OPENWEATHERMAP_COUNTRY;

    // Buffer for fetched JSON object containing weather data
    String jsonBuffer;

    // Local objects
    CDebugLog debugLog;
    SHT2x sht21;

    /**************************************************************************
     * Sends a GET request to the specified web server
     *
     * Inputs:  serverName - Server name to send the GET request to
     * Output:  String - Fetched data from GET request in JSON format
     **************************************************************************/
    String httpGETRequest(const char* serverName);
};

#endif // WEATHERDATA_H
