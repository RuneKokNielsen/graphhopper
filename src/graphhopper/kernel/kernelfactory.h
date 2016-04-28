#ifndef KERNEL_FACTORY_H
#define KERNEL_FACTORY_H

class KernelFactory{


 public:

  enum LabelType {discrete, vector};

  NodeKernel *getKernel(string name, LabelType type);

};


#endif
