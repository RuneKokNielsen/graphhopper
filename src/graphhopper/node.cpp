#include "node.h"

Node::Node(int index){
  this->index = index;
}

bool Node::operator<(const Node& other){
  return tmp_d < other.tmp_d;
}
