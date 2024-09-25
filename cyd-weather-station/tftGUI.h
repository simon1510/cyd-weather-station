#ifndef TFTGUI_H
#define TFTGUI_H

#include "weatherData.h"

#include "PNGdec.h"

// ----------------------------
// Additional Libraries - each one of these will need to be installed.
// ----------------------------
#include <TFT_eSPI.h>
// A library for interfacing with LCD displays
//
// Can be installed from the library manager (Search for "TFT_eSPI")
//https://github.com/Bodmer/TFT_eSPI

class CTftGUI
{
  public:
    CTftGUI(const TFT_eSPI& tft) : tftInstance(tft)
    {
      // guiInstancePtr = this;
    }
    void begin(void);
    bool drawTestScreen(CWeatherData::LocalData *localData, CWeatherData::OnlineData *onlineData);
    bool drawHomeScreen(CWeatherData::LocalData *localData, CWeatherData::OnlineData *onlineData);

  private:
    TFT_eSPI tftInstance;
    static PNG png;
    static CTftGUI* guiInstancePtr;

    static void pngDrawWrapper(PNGDRAW *pDraw) {
      if (guiInstancePtr) {
        // Call member instance by stored pointer
        guiInstancePtr->pngDraw(pDraw);
      }
    }
    void pngDraw(PNGDRAW *pDraw);
};

#endif // TFTGUI_H
