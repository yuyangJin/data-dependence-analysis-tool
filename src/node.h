#ifndef _NODE_H
#define _NODE_H

#include <llvm/IR/Value.h>

#include <string>
#include <vector>

using namespace llvm;

enum node_type_t {
  MALLOC = 0, // include alloc/malloc/new
  LOAD = 1,   // load
  STORE = 2,  // store
  // MPI_CALL = 3,   // mpi function calls
  GEP = 4,
  NORMAL = 5 // others

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
  Node *_gep_obj_node;

public:
  Node(Value *val, std::string name) : _val(val), _name(name) {
    _vals.push_back(val);
    _latest_access = nullptr;
    _type = NORMAL;
    _gep_obj_node = nullptr;
  }

  Node(Value *val, std::string name, node_type_t type)
      : _val(val), _name(name), _type(type) {
    _vals.push_back(val);
    _latest_access = nullptr;
    _gep_obj_node = nullptr;
  }

  // Node(Value *val, node_type_t type)
  //     : _val(val), _type(type) {
  //   _vals.push_back(val);
  //   _name = getValueName(val);
  //   _latest_access = nullptr;
  //   _gep_obj_node = nullptr;
  // }

  // Node(Value *val)
  //     : _val(val) {
  //   _vals.push_back(val);
  //   _name = getValueName(val);
  //   _latest_access = nullptr;
  //   _gep_obj_node = nullptr;
  //   _type = NORMAL;
  // }

  void addValue(Value *val);
  Value *getValue();
  int getNumValue();
  Value *getValue(int i);
  const std::string &getName();

  void setLatestAccess(Value *val);
  Value *getLatestAccess();

  void setGepObjNode(Node *n);
  Node *getGepObjNode();

  std::vector<Node *> &getInNodes();
  std::vector<Node *> &getOutNodes();
  bool addInNode(Node *e);
  bool addOutNode(Node *e);
};

#endif