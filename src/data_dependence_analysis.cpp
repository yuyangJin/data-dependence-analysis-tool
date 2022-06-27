#include <llvm/Analysis/LoopInfo.h>

#include <llvm/IR/Instruction.h>
#include <llvm/IR/Instructions.h>

#include <llvm/Pass.h>

#include "data_dependence_analysis.h"
#include "utils.h"

void instAnalysis(Instruction *inst, DDG *ddg) {
  auto inst_type = getInstType(inst);
  if (inst_type == MALLOC) {
    ddg->getOrAddNode(inst, getValueName(inst), MALLOC_INS);
  } else if (inst_type == CAST) {
    CastInst *cast_inst = dyn_cast<CastInst>(inst);
    Value *operand = cast_inst->getOperand(0);
    auto operand_node = ddg->getNode(operand);
    operand_node->addValue(inst);
    auto n = ddg->addNode(inst, operand_node);
  } else if (inst_type == LOAD) {
    // LoadInst *linst = dyn_cast<LoadInst>(inst);
    // Value *loadValPtr = linst->getPointerOperand();
    // auto n = ddg->getOrAddNode(inst, getValueName(inst), LOAD_INS);
    // auto load_node = ddg->getOrAddNode(loadValPtr, getValueName(loadValPtr));
    // ddg->addEdge(load_node, n);
  } else if (inst_type == STORE) {
  } else {
    auto n = ddg->getOrAddNode(inst, getValueName(inst));
    for (Instruction::op_iterator op = inst->op_begin(), opEnd = inst->op_end();
         op != opEnd; ++op) {

      // auto operand_inst = dyn_cast<Instruction>(*op);
      // inst_type_t operand_inst_type = getInstType(operand_inst);
      // if (operand_inst_type == CAST) {
      //     auto cast_inst = dyn_cast<CastInst>(operand_inst);
      //     operand_inst = dyn_cast<Instruction>(cast_inst->getOperand(0));
      //     operand_inst_type = getInstType(operand_inst);
      // }
      // if (operand_inst_type == MALLOC) {
      //     auto malloc_node = ddg->getNode(operand_inst);
      //     auto last_access = malloc_node->getLatestAccess();
      //     if (last_access != nullptr) { // has been accessed
      //     auto last_access_node =
      //         ddg->getOrAddNode(last_access, getValueName(last_access));
      //     ddg->addEdge(last_access_node, n);
      //     } else { // first access
      //     ddg->addEdge(malloc_node, n);
      //     }
      //     malloc_node->setLatestAccess(inst);
      // } else {
      //     auto op_node = ddg->getOrAddNode(op->get(),
      //     getValueName(op->get())); ddg->addEdge(op_node, n);
      // }
      auto op_node = ddg->getOrAddNode(op->get(), getValueName(op->get()));
      ddg->addEdge(op_node, n);
    }
  }
  return;
}

llvm::Value *getLastUse(llvm::Value *V) { return nullptr; }

llvm::Value *getLastDefine(llvm::Value *V) { return nullptr; }

llvm::Value *getLastAccess(llvm::Value *V) { return nullptr; }

llvm::Value *getLastWrite(llvm::Value *V) { return nullptr; }

llvm::Value *getLastLoad(llvm::Value *V) { return nullptr; }