#ifndef THREADED_LOOPS_H
#define THREADED_LOOPS_H

#include "kernelcomputer.h"

class ThreadedLoops: public KernelComputer{

 public:
  ThreadedLoops(int nThreads);

  void computeK(double **K, vector<Graph*> *graphs, NodeKernel *kernel);

 private:
  int _nThreads;
  int _calculated;
  int _reportEveryN;
  int _nGraphs;
  int _kSize;

  void computeInterval(double **K, vector<Graph*> *graphs, NodeKernel *kernel,
                       int from, int to);
};


#endif
