#ifndef _NODE_H
#define _NODE_H

#include <llvm/IR/Value.h>

#include <string>
#include <vector>

using namespace llvm;

enum node_type_t {
  MALLOC_INS = 0, // include alloc/malloc/new
  LOAD_INS = 1,   // load
  STORE_INS = 2,  // store
  MPI_CALL = 3,   // mpi function calls
  GEP_INS = 4,
  NORMAL_INS = 5 // others

};

class Node {
private:
  // int32_t _id;
  Value *_val;
  node_type_t _type;
  std::vector<Value *> _vals; // for bitcase
  std::string _name;
  std::vector<Node *> _in_nodes;
  std::vector<Node *> _out_nodes;
  Value *_latest_access; // for alloc/malloc/new

public:
  Node(Value *val, std::string name) : _val(val), _name(name) {
    _vals.push_back(val);
    _latest_access = nullptr;
    _type = NORMAL_INS;
  }

  Node(Value *val, std::string name, node_type_t type)
      : _val(val), _name(name), _type(type) {
    _vals.push_back(val);
    _latest_access = nullptr;
  }

  void addValue(Value *val);
  Value *getValue();
  int getNumValue();
  Value *getValue(int i);
  const std::string &getName();

  void setLatestAccess(Value *val);
  Value *getLatestAccess();
  std::vector<Node *> &getInNodes();
  std::vector<Node *> &getOutNodes();
  bool addInNode(Node *e);
  bool addOutNode(Node *e);
};

#endif