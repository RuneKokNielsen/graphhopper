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

Graph::~Graph() {

  for(int i = 0; i < V.size(); i++) {
    for(int j = 0; j < width; j++) {
      delete[] M[i][j];
    }
    delete[] M[i];
  }
  delete[] M;

  for(std::vector< Node* >::iterator it = V.begin() ; it != V.end(); ++it){
    delete (*it);
  }
  V.clear();
}

void Graph::calculateM(){
  calculateM(false);
}

void Graph::calculateM(int gaps){

  for(int i=0; i<V.size(); i++){
    prepareNode(V[i], gaps);
  }

  M = new int**[V.size()];
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
          if(r->D[vi].size() > a && r->O[vi].size() > i){
            M[vi][i][j] += r->D[vi][a] * r->O[vi][i];
          }
        }
      }
    }
  }

  for(int vi=0; vi<V.size(); vi++){
    Node *v = V[vi];
    delete[] (v->D);
    delete[] (v->O);
  }

  /*
  for(int vi=0; vi<V.size(); vi++){
    cout << "Node " << vi << "(" << V[vi]->dLabel << ")\n";
    for(int i=0; i<width; i++){
      for(int j=0; j<width; j++){
        cout << M[vi][i][j] << " ";
      }
      cout << "\n";
    }
    cout << "\n";
    }*/
}



void Graph::prepareNode(Node *src, int gaps){
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


  src->tmp_dComputed = new bool[V.size()];
  src->D = new vector<int>[V.size()];
  src->O = new vector<int>[V.size()];
  for(int i=0; i<V.size(); i++){
    src->D[i] = vector<int>();
    src->D[i].push_back(1);
    src->O[i] = vector<int>();
    src->tmp_dComputed[i] = false;
    Node *v = V[i];
    if(v->tmp_d == 999999) continue;

    src->tmp_ordered[v->tmp_d].push_back(v);
    for(int j=0; j<v->tmp_parents.size(); j++){
      v->tmp_parents[j]->tmp_children.push_back(v);
      /*
      if(gaps){
        //Add grand parents
	     
        for(int k=0; k<v->tmp_parents[j]->tmp_parents.size(); k++){
          v->tmp_grandParents.push_back(v->tmp_parents[j]->tmp_parents[k]);
        }
      }
      */
    }
  }

  if(gaps > 0){
    for(int i=0; i<V.size(); i++){
      Node *v = V[i];
      vector<Node*> tmpNodes;
      vector<Node*> tmptmpNodes;

      tmptmpNodes = v -> tmp_parents;
      for (int p = 0; p < gaps; p++) {
        tmpNodes = tmptmpNodes;
        tmptmpNodes.clear();
        for(int q = 0; q < tmpNodes.size(); q++) {
          for(int r = 0; r < tmpNodes[q] -> tmp_parents.size(); r++) {
            v -> tmp_grandParents.push_back(tmpNodes[q] -> tmp_parents[r]);
            tmptmpNodes.push_back(tmpNodes[q] -> tmp_parents[r]);
            tmpNodes[q]->tmp_parents[r]->tmp_grandChildren.push_back(v);
          }
        }
      }
      tmpNodes.clear();
      tmptmpNodes.clear();
    }
    //Use grand parents as children
    for(int i=0; i<V.size(); i++){
      Node *v = V[i];
      while(!v->tmp_grandParents.empty()){
        v->tmp_parents.push_back(v->tmp_grandParents.back());
        v->tmp_grandParents.pop_back();
      }
      while(!v->tmp_grandChildren.empty()){
        v->tmp_children.push_back(v->tmp_grandChildren.back());
        v->tmp_grandChildren.pop_back();
      }
      v -> tmp_grandParents.clear();
      v -> tmp_grandChildren.clear();
    }
  }

  compDRec(src, src);

  src->O[src->index].push_back(1);
  compO(src, maxWidth);

  delete[] (src->tmp_dComputed);
  delete[] (src->tmp_ordered);
}

void Graph::compO(Node *src, int maxWidth){
  for(int i=0; i<maxWidth; i++){
    for(int j=0; j<src->tmp_ordered[i].size(); j++){
      Node *v = src->tmp_ordered[i][j];
      for(int k=0; k<v->tmp_children.size(); k++){
        Node *c = v->tmp_children[k];
        for(int h=0; h<src->O[v->index].size(); h++){
          int ic = h + 1;
          while(ic >= src->O[c->index].size()){
            src->O[c->index].push_back(0);
          }
          src->O[c->index][ic] += src->O[v->index][h];
        }
      }
    }
  }
}

vector<int> *Graph::compDRec(Node *src, Node *v){
  if(src->tmp_dComputed[v->index]) return &(src->D[v->index]);
  for(int i=0; i<v->tmp_children.size(); i++){
    Node *c = v->tmp_children[i];
    vector<int> b = *compDRec(src, c);
    int sizea = (int) src->D[v->index].size();
    int sizeb = (int) b.size();
    for(int i=0; i<sizeb; i++){
      int ia = i + 1; 
      if(ia < sizea){
        src->D[v->index][ia] += b[i];
      }else{
        src->D[v->index].push_back(b[i]);
      }
    }
  }
  src->tmp_dComputed[v->index] = true;
  return &(src->D[v->index]);
}
