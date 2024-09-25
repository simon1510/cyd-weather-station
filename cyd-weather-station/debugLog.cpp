//==========================================================================
//
//      debugLog.cpp
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
// Standard Libraries
// ----------------------------
#include <Arduino.h>

// ----------------------------
// Custom Libraries
// ----------------------------
#include "debugLog.h"

//***********************************************
//  CDebugLog::buildString()
//***********************************************
bool CDebugLog::buildString(String *output, int logLvl, const void *string, char type)
{
  bool retVal = false;

  if((NULL != output) && (NULL != string) && (true == serialInitialized))
  {
    if(logLvl >= DEBUG_LEVEL)
    {
      switch (type)
      {
        case 'i': // Integer
          *output = String(*static_cast<const int*>(string));
          retVal = true;
          break;
        case 'f': // Float
          *output = String(*static_cast<const float*>(string));
          retVal = true;
          break;
        case 'c': // Char
          *output = String(*static_cast<const char*>(string));
          retVal = true;
          break;
        case 's': // String
          *output = String(static_cast<const char*>(string));
          retVal = true;
          break;
        default:
          *output = "Unknown type";
          break;
      }
    }
  }

  return retVal;
} /* CDebugLog::buildString() */

//***********************************************
//  CDebugLog::CDebugLog()
//***********************************************
CDebugLog::CDebugLog(void)
{
  if((DEBUG_LEVEL > DEBUG_LVL_NONE) && (false == serialInitialized))
  {
    Serial.begin(115200);
    serialInitialized = true;
  }
} /* CDebugLog::CDebugLog() */

//***********************************************
//  CDebugLog::print()
//***********************************************
void CDebugLog::print(int logLvl, const void *string, char type)
{
  String printString;

  if(true == buildString(&printString, logLvl, string, type))
  {
    Serial.print(printString);
  }
} /* CDebugLog::print() */

//***********************************************
//  CDebugLog::println()
//***********************************************
void CDebugLog::println(int logLvl, const void *string, char type)
{
  String printString;

  if(true == buildString(&printString, logLvl, string, type))
  {
    Serial.println(printString);
  }
} /* CDebugLog::println() */
