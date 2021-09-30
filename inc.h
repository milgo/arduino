
#include <avr/pgmspace.h>

const char mainMenuRunStr[] PROGMEM = "Run";
const char mainMenuEditStr[] PROGMEM = "Edit";
const char mainMenuClearStr[] PROGMEM = "Clear";
const char mainMenuSetupStr[] PROGMEM = "Setup";

const char comMenuBasicStr[] PROGMEM = "Basic";
const char comMenuMoveStr[] PROGMEM = "Move";
const char comMenuAritmeticStr[] PROGMEM = "Aritmetic";
const char comMenuCompareStr[] PROGMEM = "Compare";
const char comMenuTimersStr[] PROGMEM = "Timers";
const char comMenuCounterStr[] PROGMEM = "Counters";

const char comNOPStr[] PROGMEM = "";
const char comBasicAStr[] PROGMEM = "A";
const char comBasicOStr[] PROGMEM = "O";
const char comBasicAssignStr[] PROGMEM = "=";
const char comBasicSStr[] PROGMEM = "S";
const char comBasicRStr[] PROGMEM = "R";
const char comBasicFPStr[] PROGMEM = "FP";
const char comBasicFNStr[] PROGMEM = "FN";
const char comMoveLStr[] PROGMEM = "L";
const char comMoveTStr[] PROGMEM = "T";
const char comAritAddIStr[] PROGMEM = "+I";
const char comAritSubIStr[] PROGMEM = "-I";
const char comAritMulIStr[] PROGMEM = "*I";
const char comAritDivIStr[] PROGMEM = "/I";
const char comAritAddDStr[] PROGMEM = "+D";
const char comAritSubDStr[] PROGMEM = "-D";
const char comAritMulDStr[] PROGMEM = "*D";
const char comAritDivDStr[] PROGMEM = "/D";
const char comAritAddRStr[] PROGMEM = "+R";
const char comAritSubRStr[] PROGMEM = "-R";
const char comAritMulRStr[] PROGMEM = "*R";
const char comAritDivRStr[] PROGMEM = "/R";
const char comCompEqIStr[] PROGMEM = "==I";
const char comCompDiffIStr[] PROGMEM = "<>I";
const char comCompGtIStr[] PROGMEM = ">I";
const char comCompLtIStr[] PROGMEM = "<I";
const char comCompGtEqIStr[] PROGMEM = ">=I";
const char comCompLtEqIStr[] PROGMEM = "<=I";
const char comCompEqDStr[] PROGMEM = "==D";
const char comCompDiffDStr[] PROGMEM = "<>D";
const char comCompGtDStr[] PROGMEM = ">D";
const char comCompLtDStr[] PROGMEM = "<D";
const char comCompGtEqDStr[] PROGMEM = ">=D";
const char comCompLtEqDStr[] PROGMEM = "<=D";
const char comCompEqRStr[] PROGMEM = "==R";
const char comCompDiffRStr[] PROGMEM = "<>R";
const char comCompGtRStr[] PROGMEM = ">R";
const char comCompLtRStr[] PROGMEM = "<R";
const char comCompGtEqRStr[] PROGMEM = ">=R";
const char comCompLtEqRStr[] PROGMEM = "<=R";

const char comTimerSPStr[] PROGMEM = "SP";
const char comTimerSEStr[] PROGMEM = "SE";
const char comTimerSDStr[] PROGMEM = "SD";
const char comTimerSSStr[] PROGMEM = "SS";
const char comTimerSFStr[] PROGMEM = "SF";
const char comTimerRStr[] PROGMEM = "R";

const char comCounterCUStr[] PROGMEM = "CU";
const char comCounterCDStr[] PROGMEM = "CD";
const char comCounterStr[] PROGMEM = "S";
const char comCounterRStr[] PROGMEM = "R";
const char comCounterLStr[] PROGMEM = "L";
const char comCounterLCStr[] PROGMEM = "LC";

const char memNULLStr[] PROGMEM = "";
const char memQ0Str[] PROGMEM = "Q0";
const char memI0Str[] PROGMEM = "I0";
const char memM0Str[] PROGMEM = "M0";
const char memM1Str[] PROGMEM = "M1";
const char memM2Str[] PROGMEM = "M2";
const char memM3Str[] PROGMEM = "M3";
const char memMBStr[] PROGMEM = "MB";
const char memMWStr[] PROGMEM = "MW";
const char memMDStr[] PROGMEM = "MD";
const char memCStr[] PROGMEM = "C";
const char memTStr[] PROGMEM = "T";
const char memConstStr[] PROGMEM = "#";

const char *const mainMenu[] PROGMEM = {
  mainMenuRunStr, 
  mainMenuEditStr, 
  mainMenuClearStr, 
  mainMenuSetupStr
  };

const char *const commandGroupMenu[] PROGMEM = {
  comMenuBasicStr, 
  comMenuMoveStr, 
  comMenuTimersStr, 
  comMenuCounterStr,
  comMenuAritmeticStr, 
  comMenuCompareStr
  };

const char *const comStr[] PROGMEM = {
  comNOPStr,
  comBasicAStr,
  comBasicOStr,
  comBasicAssignStr,
  comBasicSStr,
  comBasicRStr,
  comBasicFPStr,
  comBasicFNStr,
  comMoveLStr,
  comMoveTStr,
  comTimerSPStr,
  comTimerSEStr,
  comTimerSDStr,
  comTimerSSStr,
  comTimerSFStr,
  comTimerRStr,
  comCounterCUStr,
  comCounterCDStr,
  comCounterStr,
  comCounterRStr,
  comCounterLStr,
  comCounterLCStr,
  comAritAddIStr,
  comAritSubIStr,
  comAritMulIStr,
  comAritDivIStr,
  comAritAddDStr,
  comAritSubDStr,
  comAritMulDStr,
  comAritDivDStr,
  comAritAddRStr,
  comAritSubRStr,
  comAritMulRStr,
  comAritDivRStr,
  comCompEqIStr,
  comCompDiffIStr,
  comCompGtIStr,
  comCompLtIStr,
  comCompGtEqIStr,
  comCompLtEqIStr,
  comCompEqDStr,
  comCompDiffDStr,
  comCompGtDStr,
  comCompLtDStr,
  comCompGtEqDStr,
  comCompLtEqDStr,
  comCompEqRStr,
  comCompDiffRStr,
  comCompGtRStr,
  comCompLtRStr,
  comCompGtEqRStr,
  comCompLtEqRStr,
  };

char comGroups[] = { 1, 8, 8, 10, 10, 16, 16, 22, 22, 34, 34, 52};

const char *const memStr[] PROGMEM = {
  memNULLStr,
  memQ0Str,
  memI0Str,
  memM0Str,
  memM1Str,
  memM2Str,
  memM3Str,
  memMBStr,
  memMWStr,
  memMDStr,
  memConstStr,
  memCStr,
  memTStr,
};

char memGroups[] = {1, 7, 7, 11, 12, 12, 11, 11, 0, 0, 0, 0};

char bufferStr[30];
