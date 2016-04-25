#include "node.h"

Node::Node(int index, int n){
  this->index = index;
  this->n = n;

  M = new boost::numeric::ublas::coordinate_matrix<int>(n, n);
}

bool Node::operator<(const Node& other){
  return tmp_d > other.tmp_d;
}


void Node::dumpM(){
  cout << "\nDumping M for node " << index << "\n";
  for(int i=0; i<n; i++){
    for(int j=0; j<n; j++){
      cout << (*M)(i,j) << "|";
    }
    cout << "\n\n";
  }
}
