#ifndef NODE_H
#define NODE_H


#include <vector>
#include <boost/numeric/ublas/matrix_sparse.hpp>
#include <boost/numeric/ublas/io.hpp>
using namespace std;

class Node{

 public:

  Node(int index, int n);

  bool operator<(const Node& other);


  int index;
  vector<Node*> adj;
  vector<vector<Node*> > sp;
  boost::numeric::ublas::coordinate_matrix<int> *M;
  int n;

  void dumpM();

  /*
   * Temporary variables used while calculating
   * shortest paths
   */
  int tmp_d;
  vector<vector<Node*> > tmp_sp;
};


#endif
