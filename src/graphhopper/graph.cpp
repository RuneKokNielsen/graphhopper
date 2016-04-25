#include "graph.h"
#include "node.cpp"
#include <climits>
#include <queue>

Graph::Graph(int n){
  V.reserve(n);
  for(int i=0; i<n; i++){
    V.push_back(new Node(i));
  }
}

void Graph::determineShortestPaths(){
  for(int i=0; i<V.size(); i++){
    determineShortestPathsFrom(V[i]);
  }
}

void Graph::determineShortestPathsFrom(Node *src){
  //Init tmp values
  src->tmp_d = 0;
  vector<Node*> sp;
  sp.push_back(src);
  src->tmp_sp.push_back(sp);
  vector<Node*> q;
  q.push_back(src);
  for(int i=0; i<V.size(); i++){
    Node *v = V[i];
    if(i != src->index){
      v->tmp_d = 999999;
      v->tmp_sp.clear();
      q.push_back(v);
    }
  }
  make_heap(q.begin(), q.end(), greater<Node*>());

  while(!q.empty()){
    //Issue: priority queue does not update when changing d values
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
        make_heap(q.begin(), q.end(), greater<Node*>());

        //Copy shortest paths to u and append v at end
        for(int j=0; j<u->tmp_sp.size(); j++){
          vector<Node*> p = u->tmp_sp[j];
          p.push_back(v);
          v->tmp_sp.push_back(p);
        }
      }
    }
  }

  //Save all sp's as sp's starting in src
  src->sp.clear();
  for(int i=0; i<V.size(); i++){
    Node *v = V[i];
    for(int j=0; j<v->tmp_sp.size(); j++){
      src->sp.push_back(v->tmp_sp[j]);
    }
  }
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
