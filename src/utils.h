#ifndef _UTILS_H
#define _UTILS_H

#include <llvm/IR/Function.h>
#include <llvm/IR/InstIterator.h>
#include <llvm/IR/Instruction.h>
#include <llvm/IR/Instructions.h>
#include <llvm/IR/IntrinsicInst.h>
#include <llvm/IR/Value.h>

#include <string>

#include "common.h"

enum inst_type_t {
  MALLOC_INS = 0,
  STORE_INS = 1,
  LOAD_INS = 2,
  // MPI_INS = 3,
  GEP_INS = 4,
  CAST_INS = 5,
  BIN_INS = 6,
  NO_INS = 7,
  VAL_DCL_INS = 8,
  VAL_DEF_INS = 9,
  CALL_INS = 10,
  EXT_CALL_INS = 11

};

using namespace llvm;
// If v is variable, then use the name.
// If v is instruction, then use the content.
std::string getValueName(Value *);

const MDNode *findVar(const Value *, const Function *);

std::string getDbgName(const Value *, Function *);

void printCallArgValue(CallInst *);

void printFuncArgs(Function *);

// Convert instruction to string
std::string convertIns2Str(Instruction *);
std::string convertIns2Str(Value *);

inst_type_t getInstType(Value *);

inst_type_t getInstType(Instruction *);

#endif