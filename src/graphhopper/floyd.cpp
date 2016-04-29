#include <iostream>
#include <climits>
#include <vector>

using namespace std;

int const INF = 999;

vector<vector<int> > floyd(vector<vector<int> > V) {

  vector<vector<int> > dist(V.size(), vector<int>(V.size(), INF));
  vector<vector<vector<int> > > paths(V.size(), vector<vector<int> >(V.size()));
	 
  cout << "Neighbours: " << endl;
  //Set dist to own node to 0.
  for(int v = 0; v < V.size(); v++) {
    dist[v][v] = 0;
    //Set dist to neighbours to 1.
    for(int u = 0; u < V.size(); u++) {
      dist[u][v] = V[u][v];//1;
      cout << u + 1 << " " << v + 1 << " = " << dist[u][v] << endl;
      if(dist[u][v] != INF) {
        paths[u][v].push_back(u);
	paths[u][v].push_back(v);
      }
    }
  }

  cout << "Paths: " << endl;
  for(int k = 0; k < V.size(); k++) {
    for(int i = 0; i < V.size(); i++) {
      for(int j = 0; j < V.size(); j++) {
        if(dist[i][j] > dist[i][k] + dist[k][j]) {
          dist[i][j] = dist[i][k] + dist[k][j];
	  if (paths[i][j].size() != 0) {
            paths[i][j].erase(paths[i][j].begin(), paths[i][j].end());
	  }
	  paths[i][j].push_back(i);
	  paths[i][j].push_back(k);
	  paths[i][j].push_back(j);
	  cout << k + 1 << " " << i + 1 << " " << j + 1 << " = " << dist[i][j] << endl;
	}
	//cout << k + 1 << " " << i + 1 << " " << j + 1 << " = " << dist[i][j] << endl;
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
  
  return dist;
}

int main() {
  /*
  int graph[4][4] =
    { {  0, INF,  -2, INF},
      {  4,   0,   3, INF},
      {INF, INF,   0,   2},
      {INF,  -1,  INF,  0} };
  */
  int graph[4][4] =
    { {  0,   5, INF,  10},
      {INF,   0,   3, INF},
      {INF, INF,   0,   1},
      {INF, INF, INF,   0} };

  vector<vector<int> > newGraph(4,vector<int>(4,0));
  for(int i = 0; i < newGraph.size(); i++) {
    for(int j = 0; j < newGraph[i].size(); j++) {
      newGraph[i][j] = graph[i][j];
    }
  }

  vector<vector<int> > sp = floyd(newGraph);

  /*
  for(int i = 0; i < sp.size(); i++) {
    for(int j = 0; j < sp[i].size(); j++) {
      cout << sp[i][j] << " ";
    }
    cout << endl;
  }  
  */
  return 0;
}

