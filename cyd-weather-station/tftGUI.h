#ifndef TFTGUI_H
#define TFTGUI_H

//==========================================================================
//
//      tftGUI.h
//
//      A module to draw user defined screens on a TFT display.
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
#include "weatherData.h"

// ----------------------------
// Additional Libraries - each one of these will need to be installed.
// ----------------------------
#include <TFT_eSPI.h>
// A library for interfacing with LCD displays
//
// Can be installed from the library manager (Search for "TFT_eSPI")
//https://github.com/Bodmer/TFT_eSPI

#include <PNGdec.h>
// A library for PNG decoding for MCUs with at least 48K of RAM.
//
// Can be installed from the library manager (Search for "PNGdec")

class CTftGUI
{
  public:
    /**************************************************************************
     * Standard constructor
     **************************************************************************/
    CTftGUI(const TFT_eSPI& tft) : tftInstance(tft);

    /**************************************************************************
     * Initializes display and sets all required configurations
     **************************************************************************/
    void begin(void);

    /**************************************************************************
     * Draws a test screen to the display containing all weather data
     * in raw format
     *
     * Inputs:  localData  - Pointer to stored local weather data
     *          onlineData - Pointer to stored weather data from web server
     * Output:  bool - Returns true if function call was successful
     **************************************************************************/
    bool drawTestScreen(CWeatherData::LocalData *localData, CWeatherData::OnlineData *onlineData);

    /**************************************************************************
     * Draws an overview screen to the display containing all weather data
     * in designed format with background image from background.h
     *
     * Inputs:  localData  - Pointer to stored local weather data
     *          onlineData - Pointer to stored weather data from web server
     * Output:  bool - Returns true if function call was successful
     **************************************************************************/
    bool drawHomeScreen(CWeatherData::LocalData *localData, CWeatherData::OnlineData *onlineData);

  private:

    /* Instances of required classes */
    TFT_eSPI tftInstance;
    static PNG png;
    static CTftGUI* guiInstancePtr;

    /**************************************************************************
     * Static wrapper function to call non-static pngDraw member method
     *
     * Inputs:  pDraw - Passed through pointer to wrapped method
     *                  Since pngDraw is an internal callback function, the
     *                  content of *pDraw can remain unknown
     **************************************************************************/
    static void pngDrawWrapper(PNGDRAW *pDraw)
    {
      if (guiInstancePtr)
      {
        // Call member instance by stored pointer
        guiInstancePtr->pngDraw(pDraw);
      }
    }

    /**************************************************************************
     * Callback function which is unsed internally
     *
     * Inputs:  pDraw - Passed through pointer of callback function
     *                  Since pngDraw is an internal callback function, the
     *                  content of *pDraw can remain unknown
     **************************************************************************/
    void pngDraw(PNGDRAW *pDraw);
};

#endif // TFTGUI_H
