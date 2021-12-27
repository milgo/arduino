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
#define MUST_BE_IN_RANGE 7
#define PROGRAMMING_EEPROM 8
#define READING_EEPROM 9
#define SAVE_CHANGES 10
#define NO_CHANGES 11
#define COLON 12
#define PC_CONNECTION 13

const char enterVariablePositionMsgStr[] PROGMEM = "Enter var position:";
const char enterBitPositionMsgStr[] PROGMEM = "Enter bit position:";
const char enterValueMsgStr[] PROGMEM = "Enter value:";
const char limitMsgStr[] PROGMEM = "LIMIT";
const char addLineMsgStr[] PROGMEM = "[+]";
const char runningMsgStr[] PROGMEM = "Running";
const char noProgramMsgStr[] PROGMEM = "No program";
const char mustBeInRange[] PROGMEM = "Must be in range: ";
const char programmingEepromMsgStr[] PROGMEM = "Programming eeprom...";
const char readingEepromMsgStr[] PROGMEM = "Reading from eeprom...";
const char saveChangesMsgStr[] PROGMEM = "Save changes? [ENTER]";
const char noChangesMsgStr[] PROGMEM = "No changes!";
const char colon[] PROGMEM = ": ";
const char pcConnectionMsgStr[] PROGMEM = "Connected to PC...";

const char *const message[] PROGMEM = {
  enterVariablePositionMsgStr, 
  enterValueMsgStr,
  limitMsgStr,
  addLineMsgStr,
  runningMsgStr,
  noProgramMsgStr,
  enterBitPositionMsgStr,
  mustBeInRange,
  programmingEepromMsgStr,
  readingEepromMsgStr,
  saveChangesMsgStr,
  noChangesMsgStr,
  colon,
  pcConnectionMsgStr
  };

const char mainMenuRunStr[] PROGMEM = "Run";
const char mainMenuEditStr[] PROGMEM = "Edit";
const char mainMenuProgramStr[] PROGMEM = "Program";
const char mainMenuClearStr[] PROGMEM = "Clear";

const char comMenuBasicStr[] PROGMEM = "Basic";
const char comMenuMoveStr[] PROGMEM = "Move";
const char comMenuAritmeticStr[] PROGMEM = "Aritmetic";
const char comMenuCompareStr[] PROGMEM = "Compare";
const char comMenuTimersStr[] PROGMEM = "Timers";
const char comMenuCounterStr[] PROGMEM = "Counters";
const char comMenuJumpStr[] PROGMEM = "Jump";
const char comMenuDateTimeStr[] PROGMEM = "Date/Time";

const char editMenuInsertStr[] PROGMEM = "Insert";
const char editMenuEditStr[] PROGMEM = "Edit";
const char editMenuRemoveStr[] PROGMEM = "Remove";

const char comNOPStr[] PROGMEM = "NOP";
const char comBasicAStr[] PROGMEM = "A  ";
const char comBasicOStr[] PROGMEM = "O  ";

const char comBasicANStr[] PROGMEM = "AN ";
const char comBasicONStr[] PROGMEM = "ON ";

const char comBasicAssignStr[] PROGMEM = "=  ";
const char comBasicSStr[] PROGMEM = "S  ";
const char comBasicRStr[] PROGMEM = "R  ";
const char comBasicFPStr[] PROGMEM = "FP ";
const char comBasicFNStr[] PROGMEM = "FN ";
const char comBasicPStr[] PROGMEM = "P  ";

const char comMoveLStr[] PROGMEM = "L  ";
const char comMoveTStr[] PROGMEM = "T  ";
const char comMovePStr[] PROGMEM = "P  ";

const char comAritAddIStr[] PROGMEM = "+I ";
const char comAritSubIStr[] PROGMEM = "-I ";
const char comAritMulIStr[] PROGMEM = "*I ";
const char comAritDivIStr[] PROGMEM = "/I ";
/*const char comAritAddDStr[] PROGMEM = "+D ";
const char comAritSubDStr[] PROGMEM = "-D ";
const char comAritMulDStr[] PROGMEM = "*D ";
const char comAritDivDStr[] PROGMEM = "/D ";
const char comAritAddRStr[] PROGMEM = "+R ";
const char comAritSubRStr[] PROGMEM = "-R ";
const char comAritMulRStr[] PROGMEM = "*R ";
const char comAritDivRStr[] PROGMEM = "/R ";*/
const char comCompEqIStr[] PROGMEM = "==I";
const char comCompDiffIStr[] PROGMEM = "<>I";
const char comCompGtIStr[] PROGMEM = ">I ";
const char comCompLtIStr[] PROGMEM = "<I ";
const char comCompGtEqIStr[] PROGMEM = ">=I";
const char comCompLtEqIStr[] PROGMEM = "<=I";
/*const char comCompEqDStr[] PROGMEM = "==D";
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
const char comCompLtEqRStr[] PROGMEM = "<=R";*/

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

const char comJumpUncStr[] PROGMEM = "JU ";
const char comJumpCondStr[] PROGMEM = "JC ";
const char comJumpCondNotStr[] PROGMEM = "JCN";

const char comDataTimeGetDayStr[] PROGMEM = "GDA";
const char comDataTimeGetMonthStr[] PROGMEM = "GMO";
const char comDataTimeGetYearStr[] PROGMEM = "GYE";
const char comDataTimeGetHourStr[] PROGMEM = "GHO";
const char comDataTimeGetMinutesStr[] PROGMEM = "GMI";
const char comDataTimeGetSecondsStr[] PROGMEM = "GSE";
const char comDataTimeSetDayStr[] PROGMEM = "SDA";
const char comDataTimeSetMonthStr[] PROGMEM = "SMO";
const char comDataTimeSetYearStr[] PROGMEM = "SYE";
const char comDataTimeSetHourStr[] PROGMEM = "SHO";
const char comDataTimeSetMinutesStr[] PROGMEM = "SMI";
const char comDataTimeSetSecondsStr[] PROGMEM = "SSE";

const char memNULLStr[] PROGMEM = "~";
const char memQStr[] PROGMEM = "Q";
const char memIStr[] PROGMEM = "I";
const char memMStr[] PROGMEM = "M";
const char memTStr[] PROGMEM = "T";
const char memMBStr[] PROGMEM = "MB";
const char memMWStr[] PROGMEM = "MW";
const char memMDStr[] PROGMEM = "MD";
const char memCStr[] PROGMEM = "C";
const char memConstStr[] PROGMEM = "#";
const char memAddrStr[] PROGMEM = "@";

const char *const mainMenu[] PROGMEM = {
  mainMenuRunStr, 
  mainMenuEditStr, 
  mainMenuProgramStr, 
  mainMenuClearStr
  };

const char *const commandGroupMenu[] PROGMEM = {
  comMenuBasicStr, 
  comMenuMoveStr, 
  comMenuTimersStr, 
  comMenuCounterStr,
  comMenuAritmeticStr, 
  comMenuCompareStr,
  comMenuJumpStr,
  comMenuDateTimeStr
  };

const char comGroups[] = { 1, 11, 11, 14, 14, 20, 20, 26, 26, 30, 30, 36, 36, 39, 39, 51};  
  
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
  comBasicPStr,
  
  comMoveLStr,
  comMoveTStr,
  comMovePStr,
  
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
  
  /*comAritAddDStr,
  comAritSubDStr,
  comAritMulDStr,
  comAritDivDStr,
  comAritAddRStr,
  comAritSubRStr,
  comAritMulRStr,
  comAritDivRStr,*/
  comCompEqIStr,
  comCompDiffIStr,
  comCompGtIStr,
  comCompLtIStr,
  comCompGtEqIStr,
  comCompLtEqIStr,
  /*comCompEqDStr,
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
  comCompLtEqRStr,*/
  comJumpUncStr,
  comJumpCondStr,
  comJumpCondNotStr,
  
  comDataTimeGetDayStr,
  comDataTimeGetMonthStr,
  comDataTimeGetYearStr,
  comDataTimeGetHourStr,
  comDataTimeGetMinutesStr,
  comDataTimeGetSecondsStr,
  comDataTimeSetDayStr,
  comDataTimeSetMonthStr,
  comDataTimeSetYearStr,
  comDataTimeSetHourStr,
  comDataTimeSetMinutesStr,
  comDataTimeSetSecondsStr

  };



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
  memCStr,
  memAddrStr
};

/*const char memGroups[] = {1, 5, 
                          5, 9, 
                          4, 5, 
                          9, 10, 
                          0, 0, 
                          0, 0,
                          10, 11,
                          0, 0};*/


const char memGroups[] PROGMEM = {0, 0, 
                          1, 5, 
                          1, 5, 
                          1, 5, 
                          1, 5, 
                          1, 5, 
                          1, 5, 
                          1, 5, 
                          1, 5, 
                          1, 5, 
                          3, 4, 
                          
                          5, 9, 
                          5, 8,
                          5, 8,
                          
                          4, 5, 
                          4, 5, 
                          4, 5, 
                          4, 5, 
                          4, 5, 
                          4, 5, 
                          
                          9, 10, 
                          9, 10, 
                          9, 10, 
                          9, 10, 
                          9, 10, 
                          9, 10, 
                          
                          0, 0, 
                          0, 0, 
                          0, 0, 
                          0, 0, 
                          
                          0, 0,
                          0, 0, 
                          0, 0, 
                          0, 0, 
                          0, 0, 
                          0, 0, 
                          
                          10, 11,
                          10, 11,
                          10, 11,
                          
                          0, 0,
                          0, 0,
                          0, 0,
                          0, 0,
                          0, 0,
                          0, 0,
                          0, 0,
                          0, 0,
                          0, 0,
                          0, 0,
                          0, 0,
                          0, 0};                          

const int16_t memValidationRules[] PROGMEM = {0, 0, 0, 0, 0,
                                  0, 1, 7, 0, 7,
                                  0, 1, 7, 0, 7,
                                  0, 2, 9, 0, 63,
                                  0, 1, 7, 0, 7,
                                  0, 2, 9, 0, 63,
                                  0, 2, 9, 0, 31,
                                  0, 1, 9, 0, 9,
                                  1, 5, 9, -32768, 32767,
                                  0, 1, 9, 0, 9,
                                  0, 2, 9, 0, 64};

const char memPosAquireMsg[] = {0, 0, 0, 0, 0, 0, 0, 0, 1, 0};



#endif // _MESSAGES_H
