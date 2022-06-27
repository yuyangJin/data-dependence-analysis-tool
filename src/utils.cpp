#include "utils.h"

int _val_num = 0;

// If v is variable, then use the name.
// If v is instruction, then use the content.
std::string getValueName(Value *v) {
  std::string temp_result = "#val";
  if (!v) {
    return "undefined";
  }
  if (v->getName().empty()) {
    if (isa<ConstantInt>(v)) {
      auto constant_v = dyn_cast<ConstantInt>(v);
      temp_result = std::to_string(constant_v->getSExtValue());
    } else {
      temp_result += std::to_string(_val_num);
      _val_num++;
    }
    // errs() << temp_result << "\n";
  } else {
    temp_result = v->getName().str();
    // errs() << temp_result << "\n";
  }
  return temp_result;
}

const MDNode *findVar(const Value *V, const Function *F) {
  for (const_inst_iterator Iter = inst_begin(F), End = inst_end(F); Iter != End;
       ++Iter) {
    const Instruction *I = &*Iter;
    if (const DbgDeclareInst *DbgDeclare = dyn_cast<DbgDeclareInst>(I)) {
      if (DbgDeclare->getAddress() == V) {
        return DbgDeclare->getVariable();
        // return DbgDeclare->getOperand(1);
      }
    } else if (const DbgValueInst *DbgValue = dyn_cast<DbgValueInst>(I)) {
      // errs() << "\nvalue:" <<DbgValue->getValue()  <<
      // *(DbgValue->getValue()) << "\nV:" << V << *V<< "\n";
      if (DbgValue->getValue() == V) {
        return DbgValue->getVariable();
        // return DbgValue->getOperand(1);
      }
    }
  }
  return nullptr;
}

std::string getDbgName(const Value *V, Function *F) {
  // TODO handle globals as well

  // const Function* F = findEnclosingFunc(V);
  if (!F)
    return V->getName().str();

  const MDNode *Var = findVar(V, F);
  if (!Var)
    return "tmp";

  // MDString * mds = dyn_cast_or_null<MDString>(Var->getOperand(0));
  // //errs() << mds->getString() << '\n';
  // if(mds->getString().str() != std::string("")) {
  // return mds->getString().str();
  // }else {
  // 	return "##";
  // }

  auto var = dyn_cast<DIVariable>(Var);
  // DIVariable *var(Var);

  return var->getName().str();
}

void printCallArgValue(CallInst *call_inst) {
  int num_operands = call_inst->getNumOperands();
  errs() << num_operands << "\n";
  for (int i = 0; i < num_operands; i++) {
    auto arg = call_inst->getOperand(i);
    errs() << i << ":" << *arg << "\n";
  }
}

void printFuncArgs(Function *func) {
  for (auto arg = func->arg_begin(); arg != func->arg_end(); ++arg) {
    errs() << *arg << "\n";
    // errs() << arg->getType() << "\n";
  }
}

// Convert instruction to string
std::string convertIns2Str(Instruction *ins) {
  std::string temp_str;
  raw_string_ostream os(temp_str);
  ins->print(os);
  return os.str();
}
std::string convertIns2Str(Value *ins) {
  return convertIns2Str(dyn_cast<Instruction>(ins));
}

inst_type_t getInstType(Value *inst) {
  return getInstType(dyn_cast<Instruction>(inst));
}

inst_type_t getInstType(Instruction *inst) {
  if (inst == nullptr) {
    return NO_INST;
  }
  // errs() << "type:" << *inst << '\n';
  if (isa<CallInst>(inst)) {
    auto call_inst = dyn_cast<CallInst>(inst);
    auto callee = call_inst->getCalledFunction();
    if (callee) {
      std::string callee_name = callee->getName().str();
      if (callee_name.compare(std::string("malloc")) == 0) {
        return MALLOC;
      } else if (callee_name.compare(std::string("_Znam")) == 0) {
        return MALLOC;
      } else if (startWith(callee_name, std::string("MPI"))) {
        return MPI;
      }
    }
  } else if (isa<AllocaInst>(inst)) {
    return MALLOC;
  } else if (isa<GetElementPtrInst>(inst)) {
    return GEP;
  } else if (isa<CastInst>(inst)) {
    return CAST;
  } else if (isa<LoadInst>(inst)) {
    return LOAD;
  } else if (isa<StoreInst>(inst)) {
    return STORE;
  }
  return BIN;
}