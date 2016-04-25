#include "graph.h"
#include "node.cpp"
#include <climits>
#include <queue>
#include "utils.cpp"

Graph::Graph(int n){
  V.reserve(n);
  for(int i=0; i<n; i++){
    V.push_back(new Node(i, n));
  }
}

void Graph::calculateM(){
  for(int i=0; i<V.size(); i++){
    accountFor(V[i]);
  }
}

void Graph::accountFor(Node *src){
  //Init tmp values
  vector<Node*> q;
  for(int i=0; i<V.size(); i++){
    Node *v = V[i];
    v->tmp_d = 999999;
    v->tmp_sp.clear();
    q.push_back(v);
  }
  src->tmp_d = 0;
  vector<Node*> sp;
  sp.push_back(src);
  src->tmp_sp.clear();
  src->tmp_sp.push_back(sp);

  make_heap(q.begin(), q.end(), nodeCompare);


  while(!q.empty()){
    Node *u = q.front();
    pop_heap(q.begin(), q.end());
    q.pop_back();


    int d = u->tmp_d + 1;
    for(int i=0; i<u->adj.size(); i++){
      Node *v = u->adj[i];
      if(v->tmp_d >= d){
        if(v->tmp_d > d){
          v->tmp_sp.clear();
        }
        v->tmp_d = d;
        make_heap(q.begin(), q.end(), nodeCompare);

        //Copy shortest paths to u and append v at end
        for(int j=0; j<u->tmp_sp.size(); j++){
          vector<Node*> p = u->tmp_sp[j];
          p.push_back(v);
          v->tmp_sp.push_back(p);
        }
      }
    }
  }

  // cout << "\nShortest paths starting in " << src->index << "\n";
  //Increment M_ij using these shortest paths
  for(int iv=0; iv<V.size(); iv++){
    Node *v = V[iv];
    for(int ip=0; ip<v->tmp_sp.size(); ip++){
      vector<Node*> p = v->tmp_sp[ip];
      int j = p.size()-1;
      width = max(j, width);
      //  cout << "Path of length " << j+1 << "\n";
      for(int i=0; i<=j; i++){
        // cout << v->tmp_sp[ip][i]->index << "->";
        (*(p[i]->M))(i,j)+=1;
      }
      // cout << "\n";
    }
  }

  /*
  //Save all sp's as sp's starting in src
  src->sp.clear();
  for(int i=0; i<V.size(); i++){
    Node *v = V[i];
    for(int j=0; j<v->tmp_sp.size(); j++){
      src->sp.push_back(v->tmp_sp[j]);
    }
    v->tmp_sp.clear();
    }*/
  /*
  cout << "result\n";
  for(int i=0; i<src->sp.size(); i++){
    for(int j=0; j<src->sp[i].size(); j++){
      cout << src->sp[i][j]->index << "->";
    }
    cout << "\n";
  }
  
  exit(1);*/

}
