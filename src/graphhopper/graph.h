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

  void calculateM();
  int index;
  int width = 0;
  int ***M;

 private:
  void accountFor(Node *src);
  void computeGaps(vector<int> *path, set<vector<int>> *target);
  vector<int> *compDRec(Node *src, Node *v);
  void compO(Node *src, int maxWidth);
};


#endif
