#include <llvm/Analysis/LoopInfo.h>

#include <llvm/IR/Instruction.h>
#include <llvm/IR/Instructions.h>
#include <llvm/IR/Operator.h>

#include <llvm/Pass.h>

#include "data_dependence_analysis.h"
#include "utils.h"

void instAnalysis(Instruction *inst, DDG *ddg) {
  auto inst_type = getInstType(inst);
  if (inst_type == MALLOC_INS) {
    /* ====================== MALLOC INS ======================== */
    ddg->getOrAddNode(inst, MALLOC);
  } else if (inst_type == CAST_INS) {
    /* ====================== CAST INS ======================== */

    CastInst *cast_inst = dyn_cast<CastInst>(inst);
    Value *operand = cast_inst->getOperand(0);
    auto operand_node = ddg->getOrAddNode(operand);
    operand_node->addValue(inst);
    auto n = ddg->addNode(inst, operand_node);
  } else if (inst_type == LOAD_INS) {
    /* ====================== LOAD OP ======================== */
    auto n = ddg->getOrAddNode(inst, LOAD);

    LoadInst *load_inst = dyn_cast<LoadInst>(inst);
    Value *load_val_ptr = load_inst->getPointerOperand();

    if (isa<GetElementPtrInst>(load_val_ptr)) {
      GEPOperator *gep_op = dyn_cast<GEPOperator>(load_val_ptr);
      Value *load_val = gep_op->getPointerOperand();
      // ddg->addEdge();
      auto node = ddg->getNode(load_val);
    }

    // auto load_node = ddg->getOrAddNode(load_val_ptr,
    // getValueName(loadValPtr)); ddg->addEdge(load_node, n);
  } else if (inst_type == STORE_INS) {
    /* ====================== STORE OP ======================== */
    auto n = ddg->getOrAddNode(inst, STORE);
  } else if (inst_type == GEP_INS) {
    /* ====================== GEP OP ======================== */
    auto n = ddg->getOrAddNode(inst, GEP);

    GEPOperator *gep_op = dyn_cast<GEPOperator>(inst);
    Value *gep_obj = gep_op->getPointerOperand();

    auto gep_obj_node = ddg->getNode(gep_obj);
    if (!gep_obj_node) {
      // errs() << "node is nullptr\n";
    } else {
      n->setGepObjNode(gep_obj_node);
      // auto last_access = gep_obj_node->getLatestAccess();
      // // errs() << "last_access:" << (last_access == nullptr) << "\n";
      // if (last_access) {
      //   auto last_access_node =
      //       ddg->getOrAddNode(last_access, getValueName(last_access));
      //   ddg->addEdge(last_access_node, n);
      // } else {
      //   ddg->addEdge(gep_obj_node, n);
      // }
      // gep_obj_node->setLatestAccess(inst);
    }
    GetElementPtrInst *gep_inst = dyn_cast<GetElementPtrInst>(inst);
    int num_operand = gep_inst->getNumOperands();
    for (int i = 1; i < num_operand; i++) {
      Value *op = gep_inst->getOperand(i);
      auto op_node = ddg->getOrAddNode(op);
      ddg->addEdge(op_node, n);
    }
  }
  // else if ( inst_type == NO_INS || inst_type == VAL_DCL_INS || inst_type ==
  // VAL_DEF_INS) {

  // }
  else {
    /* ====================== CAST OP ======================== */
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
      auto op_node = ddg->getOrAddNode(op->get());
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