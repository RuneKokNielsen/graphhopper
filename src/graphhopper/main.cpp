
#include <sstream>
#include <fstream>
#include <iostream>
#include <vector>
#include <string>
#include <stdlib.h>
#include "graph.cpp"
#include "utils.cpp"
#include <time.h>
#include <thread>
#include <chrono>
#include <math.h>

using namespace std;
using namespace std::chrono;

int mCompleted = 0;

void calculateM(vector<Graph*> graphs, int from, int to){
  for(int i=from; i<=to; i++){
    graphs[i]->calculateM();
    mCompleted++;
    cout << mCompleted << "\n";
  }
}
/*
void determineShortestPaths(Graph *g){
  g->determineShortestPaths();
}
*/
int main(int argc, char **argv){

  if(argc > 1){
    cout << "\nApplying GraphHopper to file: " << argv[1] << "...\n";
    steady_clock::time_point tStart;

    cout << "Loading data..\n";
    tStart = steady_clock::now();
    vector<Graph*> graphs = loadData(argv[1]);
    cout << "Data loaded in " << msPassed(tStart) << " ms\n";

    cout << "Number of graphs: " <<  graphs.size() << "\n";

    cout << "Determining shortests paths..\n";
    tStart = steady_clock::now();
    vector<thread*> threads;
    int maxThreads = 10;
    /* for(int i=0; i<graphs.size(); i++){
      thread *t = new thread(determineShortestPaths, graphs[i]);
      threads.push_back(t);
      if((i+1) == graphs.size() || (i+1) % maxThreads == 0){
        t->join();
      }
      }*/

    int chunkSize = ceil((float) graphs.size() / maxThreads);
    for(int i=0; i<maxThreads; i++){
      thread *t = new thread(calculateM, graphs, i*chunkSize, min((i+1)*chunkSize, (int) graphs.size() - 1));
      threads.push_back(t);
    }
    for(int i=0; i<maxThreads; i++){
      threads[i]->join();
    }

    cout << "M matrices determined in " << msPassed(tStart) << "ms\n";
    /*
    for(int i=0; i<graphs[0]->V.size(); i++){
      graphs[0]->V[i]->dumpM();
      }*/

  }else{
    cout << "Usage: graphhopper <file>\n";
  }

}


