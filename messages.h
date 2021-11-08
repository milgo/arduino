#ifndef _MESSAGES_H
#define _MESSAGES_H

#include <avr/pgmspace.h>

#define _FROM_0_TO_7 0x1
#define _POSITIVE_I 0x2
#define _NEGATIVE_POSITIVE_I 0x3;
#define _POSITIVE

#define ENTER_VARIABLE_POS_MSG 0
#define ENTER_VALUE_MSG 1
#define LIMIT_MSG 2
#define ADD_LINE_MSG 3
#define RUNNING_MSG 4
#define NOPROGRAM_MSG 5
#define ENTER_BIT_POS_MSG 6
#define MUST_BE_LESS_MSG 7

const char enterVariablePositionMsgStr[] PROGMEM = "Enter var position:";
const char enterBitPositionMsgStr[] PROGMEM = "Enter bit position:";
const char enterValueMsgStr[] PROGMEM = "Enter value:";
const char limitMsgStr[] PROGMEM = "LIMIT";
const char addLineMsgStr[] PROGMEM = "[+]";
const char runningMsgStr[] PROGMEM = "Running...";
const char noProgramMsgStr[] PROGMEM = "No program";
const char mustBeLessThenStr[] PROGMEM = "Value must be less then: ";

const char *const message[] PROGMEM = {
  enterVariablePositionMsgStr, 
  enterValueMsgStr,
  limitMsgStr,
  addLineMsgStr,
  runningMsgStr,
  noProgramMsgStr,
  enterBitPositionMsgStr,
  mustBeLessThenStr
  };

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

const char editMenuInsertStr[] PROGMEM = "Insert";
const char editMenuEditStr[] PROGMEM = "Edit";
const char editMenuRemoveStr[] PROGMEM = "Remove";

const char comNOPStr[] PROGMEM = "";
const char comBasicAStr[] PROGMEM = "A  ";
const char comBasicOStr[] PROGMEM = "O  ";

const char comBasicANStr[] PROGMEM = "AN ";
const char comBasicONStr[] PROGMEM = "ON ";

const char comBasicAssignStr[] PROGMEM = "=  ";
const char comBasicSStr[] PROGMEM = "S  ";
const char comBasicRStr[] PROGMEM = "R  ";
const char comBasicFPStr[] PROGMEM = "FP ";
const char comBasicFNStr[] PROGMEM = "FN ";
const char comMoveLStr[] PROGMEM = "L  ";
const char comMoveTStr[] PROGMEM = "T  ";
const char comAritAddIStr[] PROGMEM = "+I ";
const char comAritSubIStr[] PROGMEM = "-I ";
const char comAritMulIStr[] PROGMEM = "*I ";
const char comAritDivIStr[] PROGMEM = "/I ";
const char comAritAddDStr[] PROGMEM = "+D ";
const char comAritSubDStr[] PROGMEM = "-D ";
const char comAritMulDStr[] PROGMEM = "*D ";
const char comAritDivDStr[] PROGMEM = "/D ";
const char comAritAddRStr[] PROGMEM = "+R ";
const char comAritSubRStr[] PROGMEM = "-R ";
const char comAritMulRStr[] PROGMEM = "*R ";
const char comAritDivRStr[] PROGMEM = "/R ";
const char comCompEqIStr[] PROGMEM = "==I";
const char comCompDiffIStr[] PROGMEM = "<>I";
const char comCompGtIStr[] PROGMEM = ">I ";
const char comCompLtIStr[] PROGMEM = "<I ";
const char comCompGtEqIStr[] PROGMEM = ">=I";
const char comCompLtEqIStr[] PROGMEM = "<=I";
const char comCompEqDStr[] PROGMEM = "==D";
const char comCompDiffDStr[] PROGMEM = "<>D";
const char comCompGtDStr[] PROGMEM = ">D ";
const char comCompLtDStr[] PROGMEM = "<D ";
const char comCompGtEqDStr[] PROGMEM = ">=D";
const char comCompLtEqDStr[] PROGMEM = "<=D";
const char comCompEqRStr[] PROGMEM = "==R";
const char comCompDiffRStr[] PROGMEM = "<>R";
const char comCompGtRStr[] PROGMEM = ">R ";
const char comCompLtRStr[] PROGMEM = "<R ";
const char comCompGtEqRStr[] PROGMEM = ">=R";
const char comCompLtEqRStr[] PROGMEM = "<=R";

const char comTimerSPStr[] PROGMEM = "SP ";
const char comTimerSEStr[] PROGMEM = "SE ";
const char comTimerSDStr[] PROGMEM = "SD ";
const char comTimerSSStr[] PROGMEM = "SS ";
const char comTimerSFStr[] PROGMEM = "SF ";
const char comTimerRStr[] PROGMEM = "R  ";

const char comCounterCUStr[] PROGMEM = "CU ";
const char comCounterCDStr[] PROGMEM = "CD ";
const char comCounterStr[] PROGMEM = "S  ";
const char comCounterRStr[] PROGMEM = "R  ";
const char comCounterLStr[] PROGMEM = "L ";
const char comCounterLCStr[] PROGMEM = "LC ";

const char memNULLStr[] PROGMEM = "";
const char memQStr[] PROGMEM = "Q";
const char memIStr[] PROGMEM = "I";
const char memMStr[] PROGMEM = "M";
const char memTStr[] PROGMEM = "T";
const char memMBStr[] PROGMEM = "MB";
const char memMWStr[] PROGMEM = "MW";
const char memMDStr[] PROGMEM = "MD";
const char memCStr[] PROGMEM = "C";
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
  
const char *const editMenu[] PROGMEM = {
  editMenuInsertStr,
  editMenuEditStr,
  editMenuRemoveStr
};

const char *const comStr[] PROGMEM = {
  comNOPStr,
  comBasicAStr,
  comBasicOStr,
  comBasicANStr,
  comBasicONStr,
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

const char comGroups[] = { 1, 10, 10, 12, 12, 18, 18, 24, 24, 36, 36, 54};

const char *const memStr[] PROGMEM = {
  memNULLStr,
  memQStr,
  memIStr,
  memMStr,
  memTStr,
  memMBStr,
  memMWStr,
  memMDStr,
  memConstStr,
  memCStr
};

const char memGroups[] = {1, 5, 
                          5, 9, 
                          4, 5, 
                          9, 10, 
                          0, 0, 
                          0, 0};

const char memValidationRules[] = {0, 0, 0, 0,
                                  0, 1, 7, 7,
                                  0, 1, 7, 7,
                                  0, 2, 9, 63,
                                  0, 2, 9, 63,
                                  0, 2, 9, 31,
                                  0, 2, 9, 16,
                                  1, 9, 9, -1,
                                  0, 1, 9, 9,
                                  0, 1, 9, 9};

const char memPosAquireMsg[] = {0, 0, 0, 0, 0, 0, 0, 1, 0, 0};



#endif // _MESSAGES_H
