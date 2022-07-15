#include "node.h"

void Node::addValue(Value *val) { _vals.push_back(val); }
Value *Node::getValue() { return _val; }
int Node::getNumValue() { return _vals.size(); }
Value *Node::getValue(int i) { return _vals[i]; }
const std::string &Node::getName() { return _name; }

void Node::setLatestAccess(Value *val) { _latest_access = val; }

Value *Node::getLatestAccess() { return _latest_access; }

void Node::setGepObjNode(Node *n) { _gep_obj_node = n; }
Node *Node::getGepObjNode() { return _gep_obj_node; }

std::vector<Node *> &Node::getInNodes() { return _in_nodes; }
std::vector<Node *> &Node::getOutNodes() { return _out_nodes; }
bool Node::addInNode(Node *e) {
  _in_nodes.push_back(e);
  return true;
}
bool Node::addOutNode(Node *e) {
  _out_nodes.push_back(e);
  return true;
}