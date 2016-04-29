#include "kernelfactory.h"
#include "dirac.cpp"

NodeKernel *KernelFactory::getKernel(string name, LabelType type){
  if(name == "dirac"){
    if(type == discrete){
      return new Dirac();
    }else{
      throw std::invalid_argument("dirac can only be used with discrete labels.");
    }
  }
  throw std::invalid_argument("Unknown kernel specified: " + name);
}
