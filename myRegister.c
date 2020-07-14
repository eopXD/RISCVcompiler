#include "myRegister.h"
#include "myIntVector.h"
#include <stdlib.h>
#include <string.h>

char *zeroRegisterName = "zero";

char *intRegisterName[] = {"t0", "t1", "t2", "t3", "t4"};
char *intWorkRegisterName[] = {"t5", "t6"};

char *intRegisterName_64[] = {"t0", "t1", "t2", "t3", "t4"};
char *intWorkRegisterName_64[] = {"t5", "t6"};

char *intOtherRegisterName_64[] = {"s2", "s3", "s4",  "s5",  "s6", "s7",
                                   "s8", "s9", "s10", "s11", "fp"};

char *floatRegisterName[] = {
    "ft0", "ft1", "ft2", "ft3", "ft4", "ft5",
};

char *floatWorkRegisterName[] = {"ft6", "ft7"};

IntRegisterTable g_intRegisterTable;
FloatRegisterTable g_floatRegisterTable;
PseudoRegisterTable g_pseudoRegisterTable;
int g_pseudoRegisterBeginOffset = -4;

void initializeRegisterTable() {
  g_pseudoRegisterTable.isAllocatedVector = getMyIntVector(10);
}

void resetRegisterTable(int maxLocalVariableOffset) {
  memset(g_intRegisterTable.isAllocated, 0,
         sizeof(g_intRegisterTable.isAllocated));
  memset(g_floatRegisterTable.isAllocated, 0,
         sizeof(g_floatRegisterTable.isAllocated));
  memset(g_pseudoRegisterTable.isAllocatedVector->data, 0,
         sizeof(int) * g_pseudoRegisterTable.isAllocatedVector->capacity);
  g_pseudoRegisterTable.isAllocatedVector->size = 0;
  g_pseudoRegisterBeginOffset = maxLocalVariableOffset - 4;
}

int getRegister(ProcessorType processorType) {
  int realTableIndex = 0;
  int realRegisterCount =
      (processorType == INT_REG) ? INT_REGISTER_COUNT : FLOAT_REGISTER_COUNT;
  int *realRegIsAllocated = (processorType == INT_REG)
                                ? g_intRegisterTable.isAllocated
                                : g_floatRegisterTable.isAllocated;
  for (realTableIndex = 0; realTableIndex < realRegisterCount;
       ++realTableIndex) {
    if (!realRegIsAllocated[realTableIndex]) {
      realRegIsAllocated[realTableIndex] = 1;
      return realTableIndex;
    }
  }

  int pseudoTableIndex = 0;
  for (pseudoTableIndex = 0;
       pseudoTableIndex < g_pseudoRegisterTable.isAllocatedVector->size;
       ++pseudoTableIndex) {
    if (!g_pseudoRegisterTable.isAllocatedVector->data[pseudoTableIndex]) {
      g_pseudoRegisterTable.isAllocatedVector->data[pseudoTableIndex] = 1;
      return (processorType == INT_REG)
                 ? (INT_REGISTER_COUNT + pseudoTableIndex)
                 : (FLOAT_REGISTER_COUNT + pseudoTableIndex);
    }
  }

  myPushBack(g_pseudoRegisterTable.isAllocatedVector, 1);

  return (processorType == INT_REG) ? (INT_REGISTER_COUNT + pseudoTableIndex)
                                    : (FLOAT_REGISTER_COUNT + pseudoTableIndex);
}

void freeRegister(ProcessorType processorType, int registerIndex) {
  int realRegisterCount =
      (processorType == INT_REG) ? INT_REGISTER_COUNT : FLOAT_REGISTER_COUNT;
  int *realRegIsAllocated = (processorType == INT_REG)
                                ? g_intRegisterTable.isAllocated
                                : g_floatRegisterTable.isAllocated;

  if (registerIndex < realRegisterCount) {
    // free real register
    realRegIsAllocated[registerIndex] = 0;
    if (processorType == INT_REG) {
      g_intRegisterTable.is64[registerIndex] = 0;
    }
  } else {
    // free pseudo register
    int pseudoTableIndex = registerIndex - realRegisterCount;
    g_pseudoRegisterTable.isAllocatedVector->data[pseudoTableIndex] = 0;
  }
}

void printStoreRegister(FILE *codeGenOutputFp) {
  int index = 0;
  int tmpOffset = 8;
  for (index = 0; index < INT_REGISTER_COUNT; ++index) {
    fprintf(codeGenOutputFp, "sd %s,%d(sp)\n", intRegisterName_64[index],
            tmpOffset);
    tmpOffset += 8;
  }
  for (index = 0; index < INT_WORK_REGISTER_COUNT; ++index) {
    fprintf(codeGenOutputFp, "sd %s,%d(sp)\n", intWorkRegisterName_64[index],
            tmpOffset);
    tmpOffset += 8;
  }
  for (index = 0; index < INT_OTHER_REGISTER_COUNT; ++index) {
    fprintf(codeGenOutputFp, "sd %s,%d(sp)\n", intOtherRegisterName_64[index],
            tmpOffset);
    tmpOffset += 8;
  }

  for (index = 0; index < FLOAT_REGISTER_COUNT; ++index) {
    fprintf(codeGenOutputFp, "fsw %s,%d(sp)\n", floatRegisterName[index],
            tmpOffset);
    tmpOffset += 4;
  }
  for (index = 0; index < FLOAT_WORK_REGISTER_COUNT; ++index) {
    fprintf(codeGenOutputFp, "fsw %s,%d(sp)\n", floatWorkRegisterName[index],
            tmpOffset);
    tmpOffset += 4;
  }
}

void printRestoreRegister(FILE *codeGenOutputFp) {
  int index = 0;
  int tmpOffset = 8;
  for (index = 0; index < INT_REGISTER_COUNT; ++index) {
    fprintf(codeGenOutputFp, "ld %s,%d(sp)\n", intRegisterName_64[index],
            tmpOffset);
    tmpOffset += 8;
  }
  for (index = 0; index < INT_WORK_REGISTER_COUNT; ++index) {
    fprintf(codeGenOutputFp, "ld %s,%d(sp)\n", intWorkRegisterName_64[index],
            tmpOffset);
    tmpOffset += 8;
  }
  for (index = 0; index < INT_OTHER_REGISTER_COUNT; ++index) {
    fprintf(codeGenOutputFp, "ld %s,%d(sp)\n", intOtherRegisterName_64[index],
            tmpOffset);
    tmpOffset += 8;
  }

  for (index = 0; index < FLOAT_REGISTER_COUNT; ++index) {
    fprintf(codeGenOutputFp, "flw %s,%d(sp)\n", floatRegisterName[index],
            tmpOffset);
    tmpOffset += 4;
  }
  for (index = 0; index < FLOAT_WORK_REGISTER_COUNT; ++index) {
    fprintf(codeGenOutputFp, "flw %s,%d(sp)\n", floatWorkRegisterName[index],
            tmpOffset);
    tmpOffset += 4;
  }
}

int getPseudoRegisterCorrespondingOffset(int pseudoRegisterIndex) {
  return g_pseudoRegisterBeginOffset - pseudoRegisterIndex * 4;
}
