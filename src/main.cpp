#include <llvm/Pass.h>

#include "llvm/IR/DerivedUser.h"
#include "llvm/IR/GetElementPtrTypeIterator.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/InstIterator.h"
#include "llvm/IR/IntrinsicInst.h"
#include <llvm/IR/BasicBlock.h>
#include <llvm/IR/Constant.h>
#include <llvm/IR/Function.h>
#include <llvm/IR/Instruction.h>
#include <llvm/IR/Instructions.h>
#include <llvm/IR/Module.h>
#include <llvm/IR/Operator.h>
#include <llvm/IR/Use.h>
#include <llvm/IR/User.h>
#include <llvm/IR/Value.h>

#include <llvm/Analysis/CFG.h>
#include <llvm/Analysis/CallGraphSCCPass.h>
#include <llvm/Analysis/LoopInfo.h>
#include <llvm/Analysis/LoopPass.h>
#include <llvm/Analysis/ScalarEvolution.h>

#include <llvm/Support/raw_ostream.h>
//#include <llvm/DebugInfo.h>

#include <list>
#include <map>
#include <sstream>
#include <string>

#include "data_dependence_analysis.h"

// using namespace llvm;
namespace {

struct DDAPass : public ModulePass {
public:
  static char ID;

  // std::map<std::string, DDG*> _func_ddg;
  std::map<Function *, DDG *> _func_ddg;

  DDAPass() : ModulePass(ID) {}

  void getAnalysisUsage(AnalysisUsage &AU) const override {
    // AU.addRequired<CFG>();
    AU.setPreservesCFG();
    AU.addRequired<LoopInfoWrapperPass>();
    AU.addRequired<ScalarEvolutionWrapperPass>();
    // AU.addRequired<RegionInfo>();
    ModulePass::getAnalysisUsage(AU);
  }

  void handleBasicBlock(BasicBlock *curBB, DDG *ddg) {
    // Traverse all instructions
    for (BasicBlock::iterator II = curBB->begin(), IEnd = curBB->end();
         II != IEnd; ++II) {
      Instruction *curII = &*II;
      instAnalysis(curII, ddg);
    }
  }

  void handleLoop(Loop *L, LoopInfo &LI, DataLayout *DL, Function *F,
                  DDG *ddg) {

    Loop::block_iterator BB, BEnd;
    for (BB = L->block_begin(), BEnd = L->block_end(); BB != BEnd; ++BB) {
      BasicBlock *curBB = *BB;
      // check if bb belongs to L or inner loop, traverse the BB of Loop
      // itself.
      if (L == LI.getLoopFor(curBB)) {
        handleBasicBlock(curBB, ddg);
      } else {
        break;
      }
    }

    // traverse the inner Loops by recursive method
    std::vector<Loop *> subLoops = L->getSubLoops();
    Loop::iterator SL, SLE;
    for (SL = subLoops.begin(), SLE = subLoops.end(); SL != SLE; ++SL) {
      handleLoop(*SL, LI, DL, F, ddg);
    }

    for (; BB != BEnd; ++BB) {
      BasicBlock *curBB = *BB;
      // check if bb belongs to L or inner loop, traverse the BB of Loop
      // itself.
      if (L == LI.getLoopFor(curBB)) {
        handleBasicBlock(curBB, ddg);
      }
    }
  }

  DDG *handleFunction(Function *F, Module &M) {

    DDG *ddg = new DDG();

    LoopInfo &LI = getAnalysis<LoopInfoWrapperPass>(*F).getLoopInfo();
    // ScalarEvolution &SE =
    // getAnalysis<ScalarEvolutionWrapperPass>(*F).getSE();

    DataLayout *DL = new DataLayout(&M);
    Function::iterator BB, BEnd;
    for (BB = F->begin(), BEnd = F->end(); BB != BEnd; ++BB) {
      BasicBlock *curBB = &*BB;
      // check if bb belongs to any loops
      if (nullptr == LI.getLoopFor(curBB)) {
        handleBasicBlock(curBB, ddg);
      } else {
        break;
      }
    }

    for (LoopInfo::iterator LL = LI.begin(), LEnd = LI.end(); LL != LEnd;
         ++LL) {
      Loop *L = *LL;
      handleLoop(L, LI, DL, F, ddg);
    }

    for (; BB != BEnd; ++BB) {
      BasicBlock *curBB = &*BB;
      // check if bb belongs to any loops
      if (nullptr == LI.getLoopFor(curBB)) {
        handleBasicBlock(curBB, ddg);
      }
    }

    return ddg;
  }

  /* Intra-procedural analysis
   * Analyze the data dependence inside a function.
   */
  void intraProceduralAnalysis(Module &M) {
    for (auto &F : M) {
      if (!(F.isDeclaration())) {
        auto ddg = handleFunction(&F, M);
        // std::string func_name = F.getName().str();
        _func_ddg[&F] = ddg;
      }
    }
    return;
  }

  /* Inter-procedural analysis
   * Analyze the data dependence between function calls and callees.
   */
  void interProceduralAnalysis(Module &M) { return; }

  void dataDependenceAnalysis(Module &M) {
    intraProceduralAnalysis(M);
    interProceduralAnalysis(M);
  }

  bool runOnModule(Module &M) override {

    dataDependenceAnalysis(M);

    /* ================================= */
    /*            Do some test           */

    int i = 0;
    for (auto &func_ddg_pair : _func_ddg) {
      std::string file_name_str = std::to_string(i) + ".dot";
      func_ddg_pair.second->dumpGraph(file_name_str,
                                      func_ddg_pair.first->getName().str());
      i++;
    }

    /* ================================= */

    return true;
  }
};
} // namespace

char DDAPass::ID = 0;
static RegisterPass<DDAPass> X("DDAPass", "DDA Pass Analyze", false, false);