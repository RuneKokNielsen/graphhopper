
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
#include <boost/lockfree/queue.hpp>
#include <atomic>

using namespace std;
using namespace std::chrono;

int mCompleted = 0;


void calculateM(vector<Graph*> graphs, int from, int to){
  int nGraphs = graphs.size();
  int reportEveryN = nGraphs / 30;

  for(int i=from; i<=to; i++){
    graphs[i]->calculateM();
    mCompleted++;
    if(mCompleted % reportEveryN == 0 || mCompleted == nGraphs){
       cout << "M calculated: " <<  mCompleted << "/" << nGraphs <<  "\n";
    }
  }
}

atomic<bool> qlock(false);
bool popQueue(boost::lockfree::queue<Graph*> *queue, Graph **target){
  bool expected = false;
  while(!qlock.compare_exchange_weak(expected, true)){

  }
  bool result = queue->pop(*target);
  qlock = false;
  return result;
}

void calculateMQueue(boost::lockfree::queue<Graph*> *queue, int nGraphs){
  Graph *g;
  int reportEveryN = nGraphs / nGraphs;
  while(popQueue(queue, &g)){
    g->calculateM();
    mCompleted++;
    if(mCompleted % reportEveryN == 0 || mCompleted == nGraphs){
      cout << "M calculated: " <<  mCompleted << "/" << nGraphs <<  "\n";
    }
  }

}

int main(int argc, char **argv){

  if(argc > 1){
    cout << "\nApplying GraphHopper to file: " << argv[1] << "...\n";
    steady_clock::time_point tStart;
    steady_clock::time_point tStartTotal = steady_clock::now();

    cout << "Loading data..\n";
    tStart = steady_clock::now();
    vector<Graph*> graphs = loadData(argv[1]);
    int nGraphs = graphs.size();
    cout << "Data loaded in " << msPassed(tStart) << " ms\n";

    cout << "Number of graphs: " <<  graphs.size() << "\n";

    cout << "Calculating M matrices..\n";
    tStart = steady_clock::now();
    boost::lockfree::queue<Graph*> *queue =
      new boost::lockfree::queue<Graph*>(nGraphs);

    for(int i=0; i<nGraphs; i++){
      queue->push(graphs[i]);
    }
    vector<thread*> threads;
    int maxThreads = min(1, (int) nGraphs);
    for(int i=0; i<maxThreads; i++){
      thread *t = new thread(calculateMQueue, queue, nGraphs);
      threads.push_back(t);
    }
    while(mCompleted<nGraphs);

    cout << "M matrices calculated in " << msPassed(tStart) << "ms\n";

    cout << "Allocate K (" << nGraphs << "X" << nGraphs << ")..\n";
    tStart = steady_clock::now();
    int **K = new int*[nGraphs];
    for(int i=0; i<nGraphs; i++){
      K[i] = new int[nGraphs];
    }
    cout << "K allocated in " << msPassed(tStart) << "ms\n";

    cout << "Calculate K..\n";
    tStart = steady_clock::now();
    int calculated = 0;
    int kSize = nGraphs * nGraphs;
    int reportEveryN = max(kSize / 100, 1);
    for(int gii=0; gii<nGraphs; gii++){
      for(int gji=0; gji<nGraphs; gji++){
        Graph *gi = graphs[gii];
        Graph *gj = graphs[gji];
        int sum = 0;
        if(gji < gii){ //Using symmetric property
          sum = K[gji][gii];
        }else{
           int width = min(gi->width, gj->width);
           for(int vi=0; vi<gi->V.size(); vi++){
             for(int vj=0; vj<gj->V.size(); vj++){
               //  cout << "Calc " << vi << ", " << vj << "\n";
               if(gi->V[vi]->label != gj->V[vj]->label) continue;
               int weight = 0;
               for(int i=0; i<width; i++){
                 for(int j=0; j<width; j++){
                   int a = gi->M[vi][i][j];
                   if(a==0) continue;
                     int b = gj->M[vj][i][j];
                     weight += a * b;
                 }
               }
               sum += weight;
             }
           }
        }
        K[gii][gji] = sum;
        calculated ++;
        if(calculated % reportEveryN == 0 || calculated == kSize){
          cout << "K calculated: " << calculated << "/" << kSize << "\n";
        }
      }
    }
    cout << "K calculated in " << msPassed(tStart) << "ms\n";
    cout << "Total time used: " << msPassed(tStartTotal) << "ms\n";

    writeKToFile(K, nGraphs);
    /*for(int i=0; i<graphs[0]->V.size(); i++){
      graphs[0]->V[i]->dumpM();
      }*/

  }else{
    cout << "Usage: graphhopper <file>\n";
  }

}


