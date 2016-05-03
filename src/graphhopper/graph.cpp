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
    for(int ri=0; ri<V.size(); ri++){
      Node *r = V[ri];
      for(int j=0; j<width; j++){
        for(int i=0; i<=j; i++){
          int a = j - i;
          if(r->tmp_D[vi].size() > a && r->tmp_O[vi].size() > i){
            M[vi][i][j] += r->tmp_D[vi][a] * r->tmp_O[vi][i];
          }
        }
      }
    }
  }

  /*for(int vi=0; vi<V.size(); vi++){
    cout << "Node " << vi << "(" << V[vi]->dLabel << ")\n";
    for(int i=0; i<width; i++){
      for(int j=0; j<width; j++){
        cout << M[vi][i][j] << " ";
      }
      cout << "\n";
    }
    cout << "\n";
  }
  */
}



void Graph::accountFor(Node *src){
  //Init tmp values
  vector<Node*> q;
  for(int i=0; i<V.size(); i++){
    Node *v = V[i];
    v->tmp_d = 999999;
    v->tmp_parents.clear();
    v->tmp_children.clear();

    q.push_back(v);
  }
  src->tmp_d = 0;

  make_heap(q.begin(), q.end(), nodeCompare);
  int maxWidth = 0;
  while(!q.empty()){
    Node *u = q.front();
    pop_heap(q.begin(), q.end());
    q.pop_back();


    int d = u->tmp_d + 1;
    for(int i=0; i<u->adj.size(); i++){
      Node *v = u->adj[i];
      if(v->tmp_d >= d){
        if(v->tmp_d > d){
          maxWidth = max(maxWidth, d);
          v->tmp_parents.clear();
        }
        v->tmp_d = d;
        v->tmp_parents.push_back(u);
      }
    }
    make_heap(q.begin(), q.end(), nodeCompare);
  }

  maxWidth++;
  width = max(width, maxWidth);
  src->tmp_ordered = new vector<Node*>[maxWidth];

  for(int i=0; i<maxWidth; i++){
    src->tmp_ordered[i] = vector<Node*>();
  }
  src->dComputed = new bool[V.size()];
  src->tmp_D = new vector<int>[V.size()];
  src->tmp_O = new vector<int>[V.size()];
  for(int i=0; i<V.size(); i++){
    src->tmp_D[i] = vector<int>();
    src->tmp_D[i].push_back(1);
    src->tmp_O[i] = vector<int>();
    src->dComputed[i] = false;
    Node *v = V[i];
    if(v->tmp_d == 999999) continue;
    src->tmp_ordered[v->tmp_d].push_back(v);
    for(int j=0; j<v->tmp_parents.size(); j++){
      v->tmp_parents[j]->tmp_children.push_back(v);
    }
  }

  compDRec(src, src);

  src->tmp_O[src->index].push_back(1);
  compO(src, maxWidth);



}

void Graph::compO(Node *src, int maxWidth){
  for(int i=0; i<maxWidth; i++){
    for(int j=0; j<src->tmp_ordered[i].size(); j++){
      Node *v = src->tmp_ordered[i][j];
      for(int k=0; k<v->tmp_children.size(); k++){
        Node *c = v->tmp_children[k];
        for(int h=0; h<src->tmp_O[v->index].size(); h++){
          int ic = h + 1;
          while(ic >= src->tmp_O[c->index].size()){
            src->tmp_O[c->index].push_back(0);
          }
          src->tmp_O[c->index][ic] += src->tmp_O[v->index][h];
        }
      }
    }
  }
}

vector<int> *Graph::compDRec(Node *src, Node *v){
  if(src->dComputed[v->index]) return &(src->tmp_D[v->index]);
  for(int i=0; i<v->tmp_children.size(); i++){
    Node *c = v->tmp_children[i];
    vector<int> b = *compDRec(src, c);
    int sizea = src->tmp_D[v->index].size();
    int sizeb = b.size();
    for(int i=0; i<sizeb; i++){
      int ia = i + 1;
      if(ia < sizea){
        src->tmp_D[v->index][ia] += b[i];
      }else{
        src->tmp_D[v->index].push_back(b[i]);
      }
    }
  }
  src->dComputed[v->index] = true;
  return &(src->tmp_D[v->index]);
}
