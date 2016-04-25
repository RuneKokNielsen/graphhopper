#ifndef GRAPH_H
#define GRAPH_H


#include <vector>
#include "node.h"
using namespace std;

class Graph{

 public:

  Graph(int n);

  vector<Node*> V;

  void calculateM();
  int width = 0;

 private:
  void accountFor(Node *src);

};


#endif
