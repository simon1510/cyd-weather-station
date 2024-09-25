#include <Arduino.h>
#include "tftGUI.h"
#include "background.h"

// ----------------------------
// Additional Libraries - each one of these will need to be installed.
// ----------------------------
#include <TFT_eSPI.h>
// A library for interfacing with LCD displays
//
// Can be installed from the library manager (Search for "TFT_eSPI")
//https://github.com/Bodmer/TFT_eSPI

#define MAX_IMAGE_WIDTH 240 // Adjust for your images

PNG CTftGUI::png;

CTftGUI* CTftGUI::guiInstancePtr = nullptr;

void CTftGUI::begin(void)
{
  // Start the TFT display and set it to black
  tftInstance.begin();
  //tftInstance.setRotation(3); //This is the display in landscape upside down
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

bool CTftGUI::drawHomeScreen(CWeatherData::LocalData *localData, CWeatherData::OnlineData *onlineData)
{
  CTftGUI::guiInstancePtr = this;

  // Draw PNG as background
  int16_t rc = png.openFLASH((uint8_t *)background, sizeof(background), CTftGUI::pngDrawWrapper);
  if (rc == PNG_SUCCESS) {
    tftInstance.startWrite();
    rc = png.decode(NULL, 0);
    tftInstance.endWrite();
    // png.close(); // not needed for memory->memory decode
  }

  return true;
}

void CTftGUI::pngDraw(PNGDRAW *pDraw)
{
  // PNG start position
  int16_t xpos = 0;
  int16_t ypos = 0;

  uint16_t lineBuffer[MAX_IMAGE_WIDTH];
  png.getLineAsRGB565(pDraw, lineBuffer, PNG_RGB565_BIG_ENDIAN, 0xffffffff);
  CTftGUI::tftInstance.pushImage(xpos, ypos + pDraw->y, pDraw->iWidth, 1, lineBuffer);
}

