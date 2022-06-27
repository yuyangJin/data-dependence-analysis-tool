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
  MALLOC = 0,
  STORE = 1,
  LOAD = 2,
  MPI = 3,
  GEP = 4,
  CAST = 5,
  BIN = 6,
  NO_INST = 7

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