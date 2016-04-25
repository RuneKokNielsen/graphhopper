#ifndef NODE_H
#define NODE_H


#include <vector>
using namespace std;

class Node{

 public:

  Node(int index);

  bool operator<(const Node& other);

  int index;
  vector<Node*> adj;
  vector<vector<Node*> > sp;

  /*
   * Temporary variables used while calculating
   * shortest paths
   */
  int tmp_d;
  vector<vector<Node*> > tmp_sp;
};


#endif
