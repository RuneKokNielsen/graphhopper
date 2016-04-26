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
  int index;
  int width = 0;
  int ***M;

 private:
  void accountFor(Node *src);

};


#endif
