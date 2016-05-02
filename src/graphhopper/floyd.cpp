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
  vector<vector<vector<vector<int> > > > paths1(g -> V.size(), vector<vector<vector<int> > >(g -> V.size()));

  cout << "Neighbours: " << endl;
  for(int v = 0; v < g -> V.size(); v++) {
    dist[v][v] = 0;
    for(int u = 0; u < g -> V[v] -> adj.size(); u++) {
      if(g -> V[v] -> adj[u] -> index != v) {
        dist[g -> V[v] -> adj[u] -> index][v] = 1;
      }
      paths[g -> V[v] -> adj[u] -> index][v].push_back(g -> V[v] -> adj[u] -> index);
      paths[g -> V[v] -> adj[u] -> index][v].push_back(v);
      vector<int> tmp;
      tmp.push_back(g -> V[v] -> adj[u] -> index);
      tmp.push_back(v);
      paths1[g -> V[v] -> adj[u] -> index][v].push_back(tmp);
      //paths1[g -> V[v] -> adj[u] -> index][v][0].push_back(g -> V[v] -> adj[u] -> index);
      //paths1[g -> V[v] -> adj[u] -> index][v][0].push_back(v);
    }
    vector<int> tmp1;
    tmp1.push_back(v);
    tmp1.push_back(v);
    paths1[v][v].push_back(tmp1);
    paths[v][v].push_back(v);
    paths[v][v].push_back(v);
    //paths1[v][v][0].push_back(v);
    //paths1[v][v][0].push_back(v);
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
	if(dist[i][j] >= dist[i][k] + dist[k][j] && dist[i][k] != 0 && dist[k][j] != 0) {
          cout << "k: " << k << " i: " << i << " j: " << j << " dist[i][j]: " << dist[i][j] << " dist[i][k]: " << dist[i][k] << " dist[k][j]: " << dist[k][j] << endl;
	  
	  for(int q = 0; q < paths1[i][k].size(); q++) {
	    vector<int> tmp;
	    for(int r = 0; r < paths1[i][k][q].size(); r++) {
	      tmp.push_back(paths1[i][k][q][r]);
	    }
            paths1[i][j].push_back(tmp);
	  }
	}
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
  /*
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
  */
  for(int k = 0; k < paths1.size(); k++) {
    cout << k << ": " << endl;
    for(int i = 0; i < paths1[k].size(); i++) {
      for(int j = 0; j < paths1[k][i].size(); j++) {
        for(int q = 0; q < paths1[k][i][j].size(); q++) {
          cout << paths1[k][i][j][q] << " ";
	}
	cout << endl;
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

