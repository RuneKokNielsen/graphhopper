
#define MEX
#define _GLIBCXX_USE_NANOSLEEP //this_thread::sleep on old gcc

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


//Allows flushing printouts to matlab console
void mexFlush(){
  mexEvalString("pause(.001);");
}


void computeM(vector<Graph*> graphs, int from, int to, int gaps, int *completed){
  for(int i=from; i<=to; i++){
    graphs[i]->calculateM(gaps);
    *completed += 1;
  }
}

void printCompleted(int nGraphs, int *completed, bool *complete){
  int i = 0;
  while(!*complete){
    int c = *completed;
    if(i < c){
      mexPrintf("M computed: %i / %i\n", c, nGraphs);
      mexFlush();
      i = c;
    }
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
  }
  mexPrintf("M computed: %i / %i (Completed) \n", nGraphs, nGraphs);
}


vector<Graph*> matlabRead(const mxArray *data, LabelType labelType) {
  mwSize nGraphs = mxGetNumberOfElements(data);

  const mxArray *am;
  const mxArray *nlc;
  const mxArray *nld;
  const mxArray *nlv;
  const double *m;
  mwIndex *ir, *jc;
  mwIndex starting_row_index, stopping_row_index, current_row_index;

  mexPrintf("Number of graphs: %i\n", nGraphs);
  mexFlush();

  vector<Graph*> graphs(nGraphs);
  Graph *g;
  double totalNodes = 0;
  int  maxNodes = 0;
  int dimensions = 0;
  for(int i = 0; i < nGraphs; i++) {
    am = mxGetField(data, i, "am");
    nlc = mxGetField(data, i, "nl");

    switch(labelType){
    case LabelType::Both:
    case LabelType::Discrete:
      nld = mxGetField(nlc, 0, "values");
      if(nld == NULL){
        nld = nlc;
        if(nld == NULL){
           mexPrintf("No labels found in \"values\"!");
           throw std::runtime_error("No labels found in \"values\"");
        }
      }
      if (labelType == LabelType::Discrete) break;
    case LabelType::Vector:
      nlv = mxGetField(nlc, 0, "vecvalues");
      if(nlv == NULL){
        mexPrintf("No labels found in \"vecvalues\"!");
        throw std::runtime_error("No labels found in \"vecvalues\"");
      }
      break;
      }

    int nNodes = (int) mxGetM(am);
    totalNodes += nNodes;
    maxNodes = max(nNodes, maxNodes);
    g = new Graph(nNodes);
    g -> index = i;

    switch(labelType) {
    case LabelType::Both:
    case LabelType::Discrete:
      dimensions = 1;
      m = mxGetPr(nld);
      for(int j = 0; j < nNodes; j++) {
        g -> V[j] -> dLabel = (int) m[j];
      }
      if(labelType == LabelType::Discrete) break;
    case LabelType::Vector:
        m = mxGetPr(nlv);
        mwSize vLength = (mwSize) mxGetN(nlv);
        dimensions = (int) vLength;
        for(int j = 0; j < nNodes; j++) {
          Node *v = g->V[j];
          for(int k=0; k<vLength; k++){
            v->vLabel.push_back(m[k * nNodes + j]);
          }
        }
        break;
    }

    if(mxIsSparse(am)) {
      int total = 0;
      ir = mxGetIr(am);
      jc = mxGetJc(am);
      m = mxGetPr(am);
      for(int j = 0; j < nNodes; j++) {
        starting_row_index = jc[j];
        stopping_row_index = jc[j + 1];
        for(current_row_index = starting_row_index;
            current_row_index < stopping_row_index;
            current_row_index++) {
          g->V[j]->adj.push_back(g->V[ir[current_row_index]]);
          g->V[j]->adj_d.push_back(m[current_row_index]);
        }
      }
    } else {
      m = mxGetPr(am);
      for(int j = 0; j < nNodes; j++) {
        for(int k = 0; k < nNodes; k++) {
          double v = m[j + k * nNodes];
          if(v > 0) {
            g -> V[j] -> adj.push_back(g -> V[k]);
            g -> V[j] -> adj_d.push_back(v);
          }
        }
      }
    }
    graphs[i] = g;
  }
  mexPrintf("Avg. number of nodes: %f\n", totalNodes / nGraphs);
  mexPrintf("Max number of nodes: %i\n", maxNodes);
  mexPrintf("Node label dimensions: %i\n", dimensions);
  return graphs;
}


void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[]) {

  try{
    string kernelType = mxArrayToString(prhs[1]);
    LabelType labelType = KernelFactory().
      getLabelType(mxArrayToString(prhs[2]));

    NodeKernel *kernel = KernelFactory().
      getKernel(kernelType,
                labelType,
		mxGetScalar(prhs[3]));


    steady_clock::time_point tStart;
    steady_clock::time_point tStartTotal = steady_clock::now();

    mexPrintf("Loading data..\n");
    mexFlush();
    tStart = steady_clock::now();
    vector<Graph*> graphs = matlabRead(prhs[0], labelType);
    int nGraphs = (int) graphs.size();

    mexPrintf("Data loaded in: %f ms\n", msPassed(tStart));
    mexPrintf("Number of graphs: %d\n", graphs.size());
    int nThreads = max((int) mxGetScalar(prhs[5]), 1);
    mexPrintf("Running on %d threads.\n", nThreads);
    mexPrintf("Computing M matrices..\n");
    mexFlush();


    int gaps = (int) mxGetScalar(prhs[4]);

    tStart = steady_clock::now();


    int *completed = new int();
    *completed = 0;
    //Print M computation status on syncronized thread
    bool *complete = new bool();
    *complete = false;
    // thread syncPrint = thread(printCompleted, nGraphs, completed, complete);

    vector<thread*> threads;
    int chunkSize = (int) ceil(nGraphs / nThreads);
    int lastEnd = -1;
    //Start M computation threads
    for(int i=0; i<nThreads; i++){
      int end = i == nThreads - 1 ? nGraphs - 1 : lastEnd + chunkSize;
      thread *t = new thread(computeM, graphs, lastEnd + 1, end, gaps, completed);
      threads.push_back(t);
      lastEnd = end;
    }
    //Wait for threads to complete
    while(!threads.empty()){
      thread *t = threads.back();
      t->join();
      threads.pop_back();
      delete t;
    }
    *complete = true;
    //syncPrint.join();

    //computeM(graphs, 0, ((int) graphs.size()) -1, (int) mxGetScalar(prhs[4]));

    mexPrintf("M matrices computed in: %f ms\n", msPassed(tStart));
    mexPrintf("Allocate K (%dX%d)..\n", nGraphs, nGraphs);
    mexFlush();

    tStart = steady_clock::now();
    double **K = new double*[nGraphs];
    for(int i=0; i<nGraphs; i++){
      K[i] = new double[nGraphs];
    }
    mexPrintf("K allocted in: %f ms\n", msPassed(tStart));
    mexPrintf("Compute K..\n");
    mexFlush();
    KernelComputer *comp = new ThreadedLoops(nThreads);

    tStart = steady_clock::now();
    comp->computeK(K, &graphs, kernel);


    mexPrintf("K computed in: %f ms\n", msPassed(tStart));
    mexPrintf("Total time used: %f ms\n", msPassed(tStartTotal));
    mexFlush();


    //Create matlab output!
    //https://se.mathworks.com/matlabcentral/newsreader/view_thread/301952
    plhs[0] = mxCreateDoubleMatrix(nGraphs, nGraphs, mxREAL);
    double *outputMatrix = (double*) mxGetPr(plhs[0]);
    double **outputMalloc;
    outputMalloc = (double**) mxMalloc(nGraphs*sizeof(*outputMalloc));
    for(int i = 0; i < nGraphs; i++) {
      outputMalloc[i] = outputMatrix + i * nGraphs;
    }
    for(int i = 0; i < nGraphs; i++) {
      for(int j = 0; j < nGraphs; j++) {
        outputMalloc[i][j] = K[i][j];
      }
    }
    mxFree(outputMalloc);

    for(int i = 0; i < nGraphs; i++) {
      delete graphs[i];
    }
    graphs.clear();

    for(int i=0; i<nGraphs; i++){
      delete[] K[i];
    }
    delete[] K;
  }catch(std::exception& e){
    mexPrintf("%f\n", e.what());
  }

}
