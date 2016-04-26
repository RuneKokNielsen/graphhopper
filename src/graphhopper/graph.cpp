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

  /*M = new int**[V.size()];
  for(int i=0; i<V.size(); i++){
    Node *v = V[i];
    M[i] = new int*[width];
    for(int j=0; j<width; j++){
      M[i][j] = new int[width];
      for(int k=0; k<width; k++){
        M[i][j][k] = (*(v->M))(j,k);
      }
    }
    delete v->M;
    }*/

  M = new int**[V.size()];
  //cout << V.size() << "\n";
  //cout << width << "\n";
  for(int vi=0; vi<V.size(); vi++){
    M[vi] = new int*[width];
    for(int i=0; i<width; i++){
      M[vi][i] = new int[width];
      for(int j=0; j<width; j++){
        M[vi][i][j] = 0;
      }
    }
  }
  for(int vi=0; vi<V.size(); vi++){
    Node *v = V[vi];
    for(int ip=0; ip<v->sp.size(); ip++){
      vector<int> p = v->sp[ip];
      int j = p.size() - 1;
      for(int i=0; i<=j; i++){
        //cout << p[i] << " -> ";
        //cout << p[i] << ", " << i << ", " << j << "\n";
        M[p[i]][i][j]++;
      }
      // cout << "\n";
    }
    v->sp.clear();
  }

  /*for(int vi=0; vi<V.size(); vi++){
    cout << "Node " << vi << "(" << V[vi]->label << ")\n";
    for(int i=0; i<width; i++){
      for(int j=0; j<width; j++){
        cout << M[vi][i][j] << " ";
      }
      cout << "\n";
    }
    cout << "\n";
    }*/

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

        //Copy shortest paths to u and append v at end
        for(int j=0; j<u->tmp_sp.size(); j++){
          vector<Node*> p = u->tmp_sp[j];
          p.push_back(v);
          v->tmp_sp.push_back(p);
        }
      }
    }
    make_heap(q.begin(), q.end(), nodeCompare);
  }

  // cout << "\nShortest paths starting in " << src->index << "\n";
  //Increment M_ij using these shortest paths
  for(int iv=0; iv<V.size(); iv++){
    Node *v = V[iv];
    for(int ip=0; ip<v->tmp_sp.size(); ip++){
      vector<Node*> p = v->tmp_sp[ip];
      vector<int> sp;
      int j = p.size()-1;
      width = max(j + 1, width);
      for(int i=0; i<=j; i++){
        sp.push_back(p[i]->index);
      }
      src->sp.push_back(sp);
    }
  }

}
