#ifndef _DATA_DEPENDENCE_ANALYSIS_H
#define _DATA_DEPENDENCE_ANALYSIS_H

#include <llvm/IR/Instruction.h>
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/Module.h>

#include <map>
#include <string>

#include "data_dependence_graph.h"

void instAnalysis(Instruction *inst, DDG *ddg);

llvm::Value *getLastUse(llvm::Value *V);

llvm::Value *getLastDefine(llvm::Value *V);

llvm::Value *getLastAccess(llvm::Value *V);

llvm::Value *getLastWrite(llvm::Value *V);

llvm::Value *getLastLoad(llvm::Value *V);

#endif // _DATA_DEPENDENCE_ANALYSIS_H