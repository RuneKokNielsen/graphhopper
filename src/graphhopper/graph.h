#ifndef GRAPH_H
#define GRAPH_H


#include <vector>
#include <set>
#include "node.h"
using namespace std;

class Graph{

 public:

  Graph(int n);

  vector<Node*> V;

  void calculateM(int gaps);
  void calculateM();
  int index;
  int width = 0;
  int ***M;

 private:
  void prepareNode(Node *src, int gaps);
  vector<int> *compDRec(Node *src, Node *v);
  void compO(Node *src, int maxWidth);
};


#endif
