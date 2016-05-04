
#include <sstream>
#include <fstream>
#include <iostream>
#include <vector>
#include <string>
#include <stdlib.h>
#include "mex.h"
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


void computeM(vector<Graph*> graphs, int from, int to){
  int nGraphs = graphs.size();
  int reportEveryN = max(nGraphs / 30, 1);

  for(int i=from; i<=to; i++){
    graphs[i]->calculateM();
    mCompleted++;
    if(mCompleted % reportEveryN == 0 || mCompleted == nGraphs){
       cout << "M computed: " <<  mCompleted << "/" << nGraphs <<  "\n";
    }
  }
}

//int main(int argc, char **argv){
void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[]) {
  
  try{
    //if(argc > 3){

    //string kernelType = argv[2];
    string kernelType = mxArrayToString(prhs[1]);
    //LabelType labelType = KernelFactory().getLabelType(argv[3]);
    LabelType labelType = KernelFactory().getLabelType(mxArrayToString(prhs[2]));

    NodeKernel *kernel = KernelFactory().
      getKernel(kernelType,
                labelType);

    //cout << "\nApplying GraphHopper to file: " << argv[1] << "...\n";
    steady_clock::time_point tStart;
    steady_clock::time_point tStartTotal = steady_clock::now();

    //cout << "Loading data..\n";
    mexPrintf("Loading data..\n");
    tStart = steady_clock::now();
    //vector<Graph*> graphs = loadData(argv[1], labelType);
    vector<Graph*> graphs = matlabRead(prhs[0], labelType);
    int nGraphs = graphs.size();
    //cout << "Data loaded in " << msPassed(tStart) << " ms\n";
    mexPrintf("Data loaded in: %f ms\n", msPassed(tStart));

    //cout << "Number of graphs: " <<  graphs.size() << "\n";
    mexPrintf("Number of graphs: %d\n", graphs.size());
    

    //cout << "Computing M matrices..\n";
    mexPrintf("Computing M matrices..\n");
    tStart = steady_clock::now();
    computeM(graphs, 0, graphs.size()-1);

    //cout << "M matrices computed in " << msPassed(tStart) << "ms\n";
    mexPrintf("M matrices computed in: %f ms\n", msPassed(tStart));

    //cout << "Allocate K (" << nGraphs << "X" << nGraphs << ")..\n";
    mexPrintf("Allocate K (%dX%d)..\n", nGraphs, nGraphs);
    tStart = steady_clock::now();
    int **K = new int*[nGraphs];
    for(int i=0; i<nGraphs; i++){
      K[i] = new int[nGraphs];
    }
    //cout << "K allocated in " << msPassed(tStart) << "ms\n";
    mexPrintf("K allocted in: %f ms\n", msPassed(tStart));

    //cout << "Compute K..\n";
    mexPrintf("Compute K..\n");
    KernelComputer *comp = new ThreadedLoops(1);

    tStart = steady_clock::now();
    comp->computeK(K, &graphs, kernel);
    //cout << "K computed in " << msPassed(tStart) << "ms\n";
    mexPrintf("K computed in: %f ms\n", msPassed(tStart));
    //cout << "Total time used: " << msPassed(tStartTotal) << "ms\n";
    mexPrintf("Total time used: %f ms\n", msPassed(tStartTotal));

    //writeKToFile(K, nGraphs);

    //Create matlab output!
    //https://se.mathworks.com/matlabcentral/newsreader/view_thread/301952
    plhs[0] = mxCreateNumericMatrix(nGraphs, nGraphs, mxINT32_CLASS, mxREAL);
    int *outputMatrix = (int*) mxGetPr(plhs[0]);
    int **outputMalloc;
    outputMalloc = (int**) mxMalloc(nGraphs*sizeof(*outputMalloc));
    for(int i = 0; i < nGraphs; i++) {
      outputMalloc[i] = outputMatrix + i * nGraphs;
    }
    for(int i = 0; i < nGraphs; i++) {
      for(int j = 0; j < nGraphs; j++) {
        outputMalloc[i][j] = K[i][j];
      }
    }
    mxFree(outputMalloc);
    
    /*for(int i=0; i<graphs[0]->V.size(); i++){
      graphs[0]->V[i]->dumpM();
      }*/
    /*
  }else{
    cout << "\nUsage: graphhopper <file> <node kernel> <label type>\n"
         << "<file>: The destination file.\n"
         << "<node kernel>: Choose between:\n"
         << "-dirac (Only discrete node labels)\n"
         << "<label type>: discrete or vector\n"
         << "\n\n";
  }
  */
  }catch(std::exception& e){
    cout << e.what();
  }

}


