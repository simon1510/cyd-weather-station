#ifndef DEBUGLOG_H
#define DEBUGLOG_H

#include "projectConfig.h"

#define DEBUG_LVL_NONE    0
#define DEBUG_LVL_ERROR   1
#define DEBUG_LVL_INFO    2

class CDebugLog
{
  public:
    CDebugLog(void);
    void print(int logLvl, const void *string, char type);
    void println(int logLvl, const void *string, char type);
  private:
    bool serialInitialized = false;
    bool buildString(String *output, int logLvl, const void *string, char type);
};

#endif // DEBUGLOG_H
