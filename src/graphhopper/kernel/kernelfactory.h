#ifndef KERNEL_FACTORY_H
#define KERNEL_FACTORY_H

#include "nodekernel.h"
#include "labeltype.h"
class KernelFactory{


 public:

  NodeKernel *getKernel(string name, LabelType type, double kernelParameter);

  NodeKernel *getKernel(string name, string labelType, double kernelParameter);

  LabelType getLabelType(string strType);

};


#endif
