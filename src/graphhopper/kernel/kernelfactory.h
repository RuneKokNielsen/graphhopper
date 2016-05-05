#ifndef KERNEL_FACTORY_H
#define KERNEL_FACTORY_H

#include "nodekernel.h"
#include "labeltype.h"
class KernelFactory{


 public:

  NodeKernel *getKernel(string name, LabelType type, double sigma);

  NodeKernel *getKernel(string name, string labelType, double sigma);

  LabelType getLabelType(string strType);

};


#endif
