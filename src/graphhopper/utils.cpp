#ifndef UTILS_CPP
#define UTILS_CPP

#include <chrono>
using namespace std::chrono;

//http://stackoverflow.com/questions/236129/split-a-string-in-c
std::vector<std::string> &split(const std::string &s, char delim, std::vector<std::string> &elems) {
    std::stringstream ss(s);
    std::string item;
    while (std::getline(ss, item, delim)) {
        elems.push_back(item);
    }
    return elems;
}


std::vector<std::string> split(const std::string &s, char delim) {
    std::vector<std::string> elems;
    split(s, delim, elems);
    return elems;
}

void dumpGraph(Graph *g){
  cout << "Dumping graph..\n";
  cout << "Nodes: " << g->V.size() << "\n";

  for(int i=0; i<g->V.size(); i++){
    Node *n = g->V[i];
    cout << "Node #" << i << "\n";
    cout << "Node edges: " <<  n->index << "->";
    for(int j=0; j<n->adj.size(); j++){
      cout << n->adj[j]->index << ",";
    }
    cout << "\nShortest paths: (" << n->sp.size() << ")\n";
    for(int j=0; j<n->sp.size(); j++){
      for(int k=0; k<n->sp[j].size(); k++){
        cout << n->sp[j][k]->index << "->";
      }
      cout << "\n";
     
    }
 exit(1);
    cout << "\n\n";
  }


}

vector<Graph*> loadData(const char *file){
  vector<Graph*> graphs;

  string line;
  ifstream input(file);
  if(input.is_open()){
    while(getline(input, line)){
      Graph *g;
      if(line.find(",") != string::npos){
        vector<string> edge = split(line, ',');
        int a = atoi(edge.at(0).c_str());
        int b = atoi(edge.at(1).c_str());
        g->V[a]->adj.push_back(g->V[b]);
      }else{
        int n = atoi(line.c_str());
        g = new Graph(n);
        graphs.push_back(g);
      }
    }
  }

  return graphs;
}

float msPassed(steady_clock::time_point tStart){
return duration_cast<duration<double>>(steady_clock::now() - tStart).count() * 1000;
}

#endif
