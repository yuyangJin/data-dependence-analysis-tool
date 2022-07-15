#include "data_dependence_graph.h"

#include "common.h"
#include "utils.h"

using namespace llvm;

Node *DDG::addNode(Value *v, std::string str, node_type_t type) {
  Node *node = new Node(v, str, type);
  nodes[v] = node;
  // nodes.push_back(node(v, str));
  return node;
}

Node *DDG::addNode(Value *v, node_type_t type) {
  Node *node = new Node(v, getValueName(v), type);
  nodes[v] = node;
  // nodes.push_back(node(v, str));
  return node;
}

Node *DDG::addNode(Value *v, std::string str) {
  Node *node = new Node(v, str);
  nodes[v] = node;
  // nodes.push_back(node(v, str));
  return node;
}

Node *DDG::addNode(Value *v) {
  Node *node = new Node(v, getValueName(v));
  nodes[v] = node;
  // nodes.push_back(node(v, str));
  return node;
}

Node *DDG::addNode(Value *v, Node *n) {
  nodes[v] = n;
  // nodes.push_back(node(v, str));
  return n;
}

Node *DDG::getNode(Value *v) { return nodes[v]; }

Node *DDG::getOrAddNode(Value *v, std::string str, node_type_t type) {
  auto node = getNode(v);
  if (!node) {
    return addNode(v, str, type);
  }
  return node;
}

Node *DDG::getOrAddNode(Value *v, std::string str) {
  auto node = getNode(v);
  if (!node) {
    return addNode(v, str);
  }
  return node;
}

Node *DDG::getOrAddNode(Value *v, node_type_t type) {
  auto node = getNode(v);
  if (!node) {
    return addNode(v, type);
  }
  return node;
}

Node *DDG::getOrAddNode(Value *v) {
  auto node = getNode(v);
  if (!node) {
    return addNode(v);
  }
  return node;
}

// void DDG::addEdge(Value * v1, std::string str1, Value * v2, std::string str2)
// {
//     edges.push_back(edge(node(v1, str1),
//                              node(v2, str2)));
// }
void DDG::addEdge(Node *n1, Node *n2) {
  n1->addOutNode(n2);
  n2->addInNode(n1);
}

void DDG::clear() {
  // edges.clear();
  nodes.clear();
}

void DDG::getDefValues(Instruction *inst, std::vector<Value *> &defs) {
  Node *n = nodes[inst];
  if (n) {
    auto use_nodes = n->getInNodes();
    if (use_nodes.size() > 0) {
      for (auto use : use_nodes) {
        defs.push_back(use->getValue());
      }
      // TODO: return all values
      // return use_nodes[0]->getValue();
    }
  }
  // return nullptr;

  // for (edge_list::iterator edge = edges.begin(), edge_end = edges.end(); edge
  // != edge_end; ++edge) {
  //     if (dyn_cast<Instruction>(edge->first.first) == inst) {
  //         return edge->second.first;
  //     }
  // }
  // return nullptr;
}

Value *DDG::getUseValue(Instruction *inst) {
  Node *n = nodes[inst];
  if (n) {
    auto use_nodes = n->getOutNodes();
    if (use_nodes.size() > 0) {
      // TODO: return all values
      return use_nodes[0]->getValue();
    }
  }
  return nullptr;

  // for (edge_list::iterator edge = edges.begin(), edge_end = edges.end(); edge
  // != edge_end; ++edge) {
  //     if (dyn_cast<Instruction>(edge->first.first) == inst) {
  //         return edge->second.first;
  //     }
  // }
  // return nullptr;
}

void DDG::dumpGraph(std::string &file_name, Function *F) {
  dumpGraph(file_name, F->getName().str());
}

void DDG::dumpGraph(std::string &file_name, const std::string &func_name) {
  std::error_code error;
  // enum sys::fs::OpenFlags OF_None;
  enum sys::fs::CreationDisposition CD_OpenAlways;
  StringRef file_name_str(file_name);
  // raw_fd_ostream file(file_name_str, error, OF_None);
  raw_fd_ostream file(file_name_str, error, CD_OpenAlways);
  // errs() << "Write\n";
  // file << "digraph \"DDG for'" + F->getName() + "\' function\" {\n";
  file << "digraph \"DDG for'" + func_name + "\' function\" {\n";

  // dump node
  for (node_list::iterator node = nodes.begin(), node_end = nodes.end();
       node != node_end; ++node) {
    if (isa<Instruction>(node->first)) {
      auto node_str = convertIns2Str(node->first);
      node_str.erase(std::remove(node_str.begin(), node_str.end(), '\"'),
                     node_str.end());

      if (node_str.find('=') != std::string::npos) {
        errs() << node_str.find('=') << '\n';
        node_str = node_str.substr(0, node_str.find('='));
        errs() << node_str.c_str() << '\n';
      }

      file << "\tNode" << node->first << "[shape=record, label=\""
           << node_str.c_str() << "\"];\n";
    }
  }

//  dump instruction edges
#ifdef CFG
  for (edge_list::iterator edge = inst_edges.begin(),
                           edge_end = inst_edges.end();
       edge != edge_end; ++edge) {
    file << "\tNode" << edge->first.first << " -> Node" << edge->second.first
         << "\n";
  }
#endif

  // dump data flow edges
  file << "edge [color=red]"
       << "\n";

  for (node_list::iterator node = nodes.begin(), node_end = nodes.end();
       node != node_end; ++node) {
    auto dest = node->first;
    if (node->second == nullptr) {
      continue;
    }
    for (auto src : node->second->getInNodes()) {
      file << "\tNode" << src->getValue() << " -> Node" << dest << "\n";
    }
  }

  file << "}\n";
  file.close();
}
