#include "node.h"

Node::Node(int index, int n){
  this->index = index;
  this->n = n;

}

bool Node::operator<(const Node& other){
  return tmp_d > other.tmp_d;
}

