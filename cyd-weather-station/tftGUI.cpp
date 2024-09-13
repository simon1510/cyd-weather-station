#include <Arduino.h>
#include "tftGUI.h"

// ----------------------------
// Additional Libraries - each one of these will need to be installed.
// ----------------------------
#include <TFT_eSPI.h>
// A library for interfacing with LCD displays
//
// Can be installed from the library manager (Search for "TFT_eSPI")
//https://github.com/Bodmer/TFT_eSPI

// CTftGUI::CTftGUI(const TFT_eSPI& tft) : tftInstance(tft)
// {
//   //instance = TFT_eSPI::getSPIinstance();
// }

void CTftGUI::begin(void)
{
  // Start the TFT display and set it to black
  tftInstance.init();
  tftInstance.setRotation(3); //This is the display in landscape upside down
  tftInstance.setTextWrap(true, true);

  // Clear the screen before writing to it and set default text colors
  tftInstance.fillScreen(TFT_BLACK);
  tftInstance.setTextColor(TFT_WHITE, TFT_BLACK);
}

bool CTftGUI::drawTestScreen(CWeatherData::LocalData *localData, CWeatherData::OnlineData *onlineData)
{
  bool retVal = false;

  if((NULL != localData) && (NULL != onlineData))
  {
    char printStr[100];

    snprintf(printStr, sizeof(printStr), "Temperature: %.2f C", onlineData->temperature);
    tftInstance.drawString(printStr, 0, 60, 2);
    snprintf(printStr, sizeof(printStr), "Feels like: %.2f C", onlineData->feels_like);
    tftInstance.drawString(printStr, 0, 90, 2);
    snprintf(printStr, sizeof(printStr), "Humidity: %d %%", onlineData->humidity);
    tftInstance.drawString(printStr, 0, 120, 2);
    snprintf(printStr, sizeof(printStr), "Air Pressure: %d hPa", onlineData->pressure);
    tftInstance.drawString(printStr, 0, 150, 2);
    snprintf(printStr, sizeof(printStr), "Wind Speed: %.2f m/s", onlineData->wind_speed);
    tftInstance.drawString(printStr, 0, 180, 2);

    retVal = true;
  }

  return retVal;
}

// TFT_eSPI& CTftGUI::tft()
// {
//     static TFT_eSPI instance;
//     return instance;
// }
