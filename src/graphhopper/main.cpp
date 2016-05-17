
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
#include "threadedloops.cpp"
#include "kernel/kernelfactory.cpp"

using namespace std;
using namespace std::chrono;

int mCompleted = 0;
bool gaps = false;


void computeM(vector<Graph*> graphs, int from, int to){
  int nGraphs = graphs.size();
  int reportEveryN = max(nGraphs / 30, 1);

  for(int i=from; i<=to; i++){
    graphs[i]->calculateM(gaps);
    mCompleted++;
    if(mCompleted % reportEveryN == 0 || mCompleted == nGraphs){
       cout << "M computed: " <<  mCompleted << "/" << nGraphs <<  "\n";
    }
  }
}

int main(int argc, char **argv){

  try{
    if(argc > 4){

    string kernelType = argv[2];
    LabelType labelType = KernelFactory().getLabelType(argv[3]);

    NodeKernel *kernel = KernelFactory().
      getKernel(kernelType,
                labelType,
		atof(argv[4]));

    gaps = argc > 5 && ((string) argv[5]) == "gaps";

    cout << "\nApplying GraphHopper to file: " << argv[1] << "...\n";
    steady_clock::time_point tStart;
    steady_clock::time_point tStartTotal = steady_clock::now();

    cout << "Loading data..\n";
    tStart = steady_clock::now();
    vector<Graph*> graphs = loadData(argv[1], labelType);
    int nGraphs = graphs.size();
    cout << "Data loaded in " << msPassed(tStart) << " ms\n";

    cout << "Number of graphs: " <<  graphs.size() << "\n";

    cout << "Computing M matrices..\n";
    tStart = steady_clock::now();
    computeM(graphs, 0, graphs.size()-1);

    cout << "M matrices computed in " << msPassed(tStart) << "ms\n";
    cout << "Allocate K (" << nGraphs << "X" << nGraphs << ")..\n";
    tStart = steady_clock::now();
    double **K = new double*[nGraphs];
    for(int i=0; i<nGraphs; i++){
      K[i] = new double[nGraphs];
    }
    cout << "K allocated in " << msPassed(tStart) << "ms\n";
    cout << "Compute K..\n";
    KernelComputer *comp = new ThreadedLoops(1);

    tStart = steady_clock::now();
    comp->computeK(K, &graphs, kernel);
    cout << "K computed in " << msPassed(tStart) << "ms\n";
    cout << "Total time used: " << msPassed(tStartTotal) << "ms\n";
    writeKToFile(K, nGraphs);

    /*for(int i=0; i<graphs[0]->V.size(); i++){
      graphs[0]->V[i]->dumpM();
      }*/
  }else{
    cout << "\nUsage: graphhopper <file> <node kernel> <label type>\n"
         << "<file>: The destination file.\n"
         << "<node kernel>: Choose between:\n"
         << "-dirac (Only discrete node labels)\n"
         << "<label type>: discrete or vector\n"
         << "<node kernel parameter> decimal\n"
         << "<gaps> (optional) Type \"gaps\" to run with gaps. Only use on trees!\n"
         << "\n\n";
  }
  }catch(std::exception& e){
    cout << e.what();
  }

}


