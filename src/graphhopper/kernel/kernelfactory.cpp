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

NodeKernel *KernelFactory::getKernel(string name, string type){
  if(type == "discrete"){
    return getKernel(name, discrete);
  }
  if(type == "vector"){
    return getKernel(name, vector);
  }

  throw std::invalid_argument(type + " does not name a proper label type. Options are discrete and vector");

}
