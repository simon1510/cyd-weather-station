#include <Arduino.h>
#include "debugLog.h"


CDebugLog::CDebugLog(void)
{
  if((DEBUG_LEVEL > DEBUG_LVL_NONE) && (false == serialInitialized))
  {
    Serial.begin(115200);
    serialInitialized = true;
  }
}

void CDebugLog::print(int logLvl, const void *string, char type)
{
  if((NULL != string) && (true == serialInitialized))
  {
    if(logLvl >= DEBUG_LEVEL)
    {
      String printString;

      switch (type)
      {
        case 'i': // Integer
          printString = String(*static_cast<const int*>(string));
          break;
        case 'f': // Float
          printString = String(*static_cast<const float*>(string));
          break;
        case 'c': // Char
          printString = String(*static_cast<const char*>(string));
          break;
        case 's': // String
          printString = String(static_cast<const char*>(string));
          break;
        default:
          printString = "Unknown type";
          break;
      }

      Serial.print(printString);
    }
  }
}

void CDebugLog::println(int logLvl, const void *string, char type)
{
  if((NULL != string) && (true == serialInitialized))
  {
    if(logLvl >= DEBUG_LEVEL)
    {
      String printString;

      switch (type)
      {
        case 'i': // Integer
          printString = String(*static_cast<const int*>(string));
          break;
        case 'f': // Float
          printString = String(*static_cast<const float*>(string));
          break;
        case 'c': // Char
          printString = String(*static_cast<const char*>(string));
          break;
        case 's': // String
          printString = String(static_cast<const char*>(string));
          break;
        default:
          printString = "Unknown type";
          break;
      }

      Serial.println(printString);
    }
  }
}
