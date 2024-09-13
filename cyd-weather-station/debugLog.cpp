#include <Arduino.h>
#include "debugLog.h"


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
}


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
  String printString;

  if(true == buildString(&printString, logLvl, string, type))
  {
    Serial.print(printString);
  }
}

void CDebugLog::println(int logLvl, const void *string, char type)
{
  String printString;

  if(true == buildString(&printString, logLvl, string, type))
  {
    Serial.println(printString);
  }
}
