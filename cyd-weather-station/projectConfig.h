#ifndef PROJECTCONFIG_H
#define PROJECTCONFIG_H

/*******************************************
 * Global debug level for serial outputs
 *
 * Supported levels:
 * 0 - NONE
 * 1 - ERROR
 * 2 - INFO
 *******************************************/
#define DEBUG_LEVEL   2

/*******************************************
 * Configuration for SHT21 sensor
 *******************************************/
// SHT21 I2C pin definitions
#define SHT21_SDA     22
#define SHT21_SCL     27

/*******************************************
 * Configuration for OpenWeatherMap
 *******************************************/
#define OPENWEATHERMAP_API_KEY  "YourAPIKey"
#define OPENWEATHERMAP_CITY     "YourCity"
#define OPENWEATHERMAP_COUNTRY  "YourCountryCode"

/*******************************************
 * WIFI Configuration
 *******************************************/
// Change this to your WiFi SSID
#define WIFI_SSID   "Your SSID"
// Change this to your WiFi password
#define WIFI_PW     "YourWifiPW"


#endif // PROJECTCONFIG_H
