#ifndef NODE_H
#define NODE_H


#include <vector>
using namespace std;

class Node{

 public:

  Node(int index, int n);

  bool operator<(const Node& other);


  int index;
  vector<Node*> adj;
  int n;
  int dLabel;
  vector<double> vLabel;

  vector<int> *D;
  vector<int> *O;
  /*
   * Temporary variables used while calculating
   * shortest paths
   */
  int tmp_d;
  vector<Node*> tmp_parents;
  vector<Node*> tmp_children;
  vector<Node*> tmp_grandChildren;
  vector<Node*> tmp_grandParents;
  bool *tmp_dComputed;
  vector<Node*> *tmp_ordered;
};


#endif
