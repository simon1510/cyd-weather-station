#ifndef DEBUGLOG_H
#define DEBUGLOG_H

//==========================================================================
//
//      debugLog.h
//
//      A logging module which enables to (de)activate logs
//      for specific log levels.
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
#include "projectConfig.h"

#define DEBUG_LVL_NONE    0
#define DEBUG_LVL_ERROR   1
#define DEBUG_LVL_INFO    2

class CDebugLog
{
  public:
    /**************************************************************************
     * Standard constructor
     **************************************************************************/
    CDebugLog(void);

    /**************************************************************************
     * Prints a log message
     *
     * Inputs:  logLvl - Log level for which the message should be printed
     *                   Maximum printed log level is defined in projektConfig.h
     *                   Allowed values are:
     *                   DEBUG_LVL_NONE, DEBUG_LVL_ERROR or DEBUG_LVL_INFO
     *          string - String to be printed
     *          type   - Type definition of string. Allowed are:
     *                   'i' - Integer
     *                   'f' - Float
     *                   'c' - Char
     *                   's' - String
     **************************************************************************/
    void print(int logLvl, const void *string, char type);

    /**************************************************************************
     * Prints a log message with a new line command at the end
     *
     * Inputs:  logLvl - Log level for which the message should be printed
     *                   Maximum printed log level is defined in projektConfig.h
     *                   Allowed values are:
     *                   DEBUG_LVL_NONE, DEBUG_LVL_ERROR or DEBUG_LVL_INFO
     *          string - String to be printed
     *          type   - Type definition of string. Allowed are:
     *                   'i' - Integer
     *                   'f' - Float
     *                   'c' - Char
     *                   's' - String
     **************************************************************************/
    void println(int logLvl, const void *string, char type);

  private:
    // Variable to indicate if serial interface has been initialized
    bool serialInitialized = false;

    /**************************************************************************
     * Builds a string to be compatible with standard print function
     * Inputs:  output - Pointer to where the built string is copied
     *          logLvl - Log level for which the message should be printed
     *                   Maximum printed log level is defined in projektConfig.h
     *                   Allowed values are:
     *                   DEBUG_LVL_NONE, DEBUG_LVL_ERROR or DEBUG_LVL_INFO
     *          string - String to be printed
     *          type   - Type definition of string. Allowed are:
     *                   'i' - Integer
     *                   'f' - Float
     *                   'c' - Char
     *                   's' - String
     **************************************************************************/
    bool buildString(String *output, int logLvl, const void *string, char type);
};

#endif // DEBUGLOG_H
