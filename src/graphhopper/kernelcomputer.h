#ifndef KERNEL_COMPUTER_H
#define KERNEL_COMPUTER_H

#include "kernel/nodekernel.h"

class KernelComputer{

 public:
  virtual void computeK(int **K, vector<Graph*> *graphs, NodeKernel *kernel) = 0;

};


#endif
