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


vector<Graph*> matlabRead(const mxArray *data, LabelType labelType) {
  mwSize nGraphs = mxGetNumberOfElements(data);

  const mxArray *am;
  const mxArray *nlc;
  const mxArray *nl;
  const double *m;
  mxArray *row;
  mwIndex *ir, *jc;
  mwIndex starting_row_index, stopping_row_index, current_row_index;

  vector<Graph*> graphs(nGraphs);
  Graph *g;
  for(int i = 0; i < nGraphs; i++) {
    am = mxGetField(data, i, "am");
    nlc = mxGetField(data, i, "nl");
    nl = mxGetField(nlc, 0, "values");
    if(nl == NULL) nl = mxGetField(nlc, 0, "vecvalues");

    mwSize nNodes = mxGetM(am);
    g = new Graph(nNodes);
    g -> index = i;

    m = mxGetPr(nl);
    //Missing vector implementation!
    switch(labelType) {
    case LabelType::Discrete:
      for(int j = 0; j < nNodes; j++) {
        g -> V[j] -> dLabel = m[j];
      }
      break;
      /*case LabelType::Vector:
      for(int j = 0; j < nNodes; j++) {
        for(int k = 0; k < nNodes; k++) {

        }
      }
      break;*/
    }

    m = mxGetPr(am);
    //if(mxIsSparse(am)) {
    //} else {
      for(int j = 0; j < nNodes; j++) {
        for(int k = 0; k < nNodes; k++) {
          double v = m[j + k * nNodes];
	  if(v > 0) {
            g -> V[j] -> adj.push_back(g -> V[k]);
	  }
	}
      }
      //}
    graphs[i] = g;
  }
  return graphs;
}


void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[]) {

  try{
    string kernelType = mxArrayToString(prhs[1]);
    LabelType labelType = KernelFactory().
      getLabelType(mxArrayToString(prhs[2]));

    NodeKernel *kernel = KernelFactory().
      getKernel(kernelType,
                labelType);

    steady_clock::time_point tStart;
    steady_clock::time_point tStartTotal = steady_clock::now();

    mexPrintf("Loading data..\n");
    tStart = steady_clock::now();
    vector<Graph*> graphs = matlabRead(prhs[0], labelType);
    int nGraphs = graphs.size();

    mexPrintf("Data loaded in: %f ms\n", msPassed(tStart));
    mexPrintf("Number of graphs: %d\n", graphs.size());

    mexPrintf("Computing M matrices..\n");
    tStart = steady_clock::now();
    computeM(graphs, 0, graphs.size()-1);

    mexPrintf("M matrices computed in: %f ms\n", msPassed(tStart));

    mexPrintf("Allocate K (%dX%d)..\n", nGraphs, nGraphs);
    tStart = steady_clock::now();
    int **K = new int*[nGraphs];
    for(int i=0; i<nGraphs; i++){
      K[i] = new int[nGraphs];
    }
    mexPrintf("K allocted in: %f ms\n", msPassed(tStart));

    mexPrintf("Compute K..\n");
    KernelComputer *comp = new ThreadedLoops(1);

    tStart = steady_clock::now();
    comp->computeK(K, &graphs, kernel);
    mexPrintf("K computed in: %f ms\n", msPassed(tStart));

    mexPrintf("Total time used: %f ms\n", msPassed(tStartTotal));


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

  }catch(std::exception& e){
    mexPrintf("%f\n", e.what());
  }

}
