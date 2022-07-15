#ifndef _DATA_DEPENDENCE_GRAPH_H
#define _DATA_DEPENDENCE_GRAPH_H

#include <llvm/IR/Function.h>
#include <llvm/IR/Instruction.h>
#include <llvm/IR/Value.h>
#include <llvm/Support/raw_ostream.h>

// #include <list>
#include <algorithm>
#include <map>
#include <vector>

#include "node.h"

using namespace llvm;

typedef std::map<Value *, Node *> node_list;

class DDG {

private:
  // std::error_code error;
  // edge_list inst_edges; // control flow
  // edge_list edges;  // data flow
  node_list nodes; // instruction
                   // DDG() {}
                   // ~DDG() {}

public:
  Node *addNode(Value *, std::string, node_type_t);
  Node *addNode(Value *, std::string);
  Node *addNode(Value *, node_type_t);
  Node *addNode(Value *);
  Node *addNode(Value *, Node *);

  Node *getNode(Value *v);

  Node *getOrAddNode(Value *, std::string, node_type_t);
  Node *getOrAddNode(Value *, std::string);
  Node *getOrAddNode(Value *, node_type_t);
  Node *getOrAddNode(Value *);

  // void addEdge(Value *, std::string, Value *, std::string);
  void addEdge(Node *n1, Node *n2);
  void clear();

  void getDefValues(Instruction *inst, std::vector<Value *> &);
  Value *getUseValue(Instruction *inst);
  void dumpGraph(std::string &file, Function *F);
  void dumpGraph(std::string &file, const std::string &func);
};

#endif // _DATA_DEPENDENCE_GRAPH_H