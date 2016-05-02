#include <sstream>
#include <fstream>
#include <iostream>
#include <climits>
#include <vector>
#include <string>
#include <stdlib.h>
#include "graph.cpp"
#include "utils.cpp"
#include "kernel/kernelfactory.cpp"

using namespace std;

int const INF = 999;

void floyd(Graph* g) {

  vector<vector<int> > dist(g -> V.size(), vector<int>(g -> V.size(), INF));
  vector<vector<vector<int> > > paths(g -> V.size(), vector<vector<int> >(g -> V.size()));
  //vector<vector<vector<vector<int> > > > paths(g -> V.size(), vector<vector<vector<int> > >(g -> V.size()));

  cout << "Neighbours: " << endl;
  for(int v = 0; v < g -> V.size(); v++) {
    dist[v][v] = 0;
    for(int u = 0; u < g -> V[v] -> adj.size(); u++) {
      if(g -> V[v] -> adj[u] -> index != v) {
        dist[g -> V[v] -> adj[u] -> index][v] = 1;
      }
      paths[g -> V[v] -> adj[u] -> index][v].push_back(g -> V[v] -> adj[u] -> index);
      paths[g -> V[v] -> adj[u] -> index][v].push_back(v);
    }
    paths[v][v].push_back(v);
    paths[v][v].push_back(v);
  }

  /*
  for(int i = 0; i < dist.size(); i++) {
    for(int j = 0; j < dist[i].size(); j++) {
      cout << dist[i][j] << " ";
    }
    cout << endl;
  }
  */
  cout << "Paths: " << endl;
  for(int k = 0; k < g -> V.size(); k++) {
    for(int i = 0; i < g -> V.size(); i++) {
      for(int j = 0; j < g -> V.size(); j++) {
	//cout << dist[i][j] << " " << dist[i][k] + dist[k][j] << endl;
	if(dist[i][j] > dist[i][k] + dist[k][j]) {
          dist[i][j] = dist[i][k] + dist[k][j];
	  if (paths[i][j].size() != 0) {
            paths[i][j].erase(paths[i][j].begin(), paths[i][j].end());
	  }
	  //paths[i][j].push_back(i);
	  for(int q = 0; q < paths[i][k].size(); q++) {
	    paths[i][j].push_back(paths[i][k][q]);
	    //cout << "k: " << k << " i: " << i << " j: " << j << " q: " << q << endl;
	  }
	  paths[i][j].push_back(j);
	  //cout << k << " " << i << " " << j << " = " << dist[i][j] << endl;
	}
      }
    }
  }
  
  cout << "Stored Paths: " << endl;
  for(int k = 0; k < paths.size(); k++) {
    cout << k << ": " << endl;
    for(int i = 0; i < paths[k].size(); i++) {
      for(int j = 0; j < paths[k][i].size(); j++) {
        cout << paths[k][i][j] << " ";
      }
      cout << endl;
    }
    cout << "----" << endl;
  }
  //return dist;
}

int main(int argc, char **argv) {
  LabelType labelType = KernelFactory().getLabelType(argv[2]);
  vector<Graph*> graphs = loadData(argv[1], labelType);
  steady_clock::time_point tStart;
  tStart = steady_clock::now();
  for(int i = 0; i < graphs.size(); i++) {
    floyd(graphs[i]);
  }
  cout << "Graphs done in: " << msPassed(tStart) << " ms\n";
  return 0;
}

