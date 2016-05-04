#include "kernelfactory.h"
#include "dirac.cpp"
#include "linear.cpp"
#include "gaussian.cpp"

NodeKernel *KernelFactory::getKernel(string name, LabelType type){
  if(name == "dirac"){
    if(type == Discrete){
      return new Dirac();
    }else{
      throw std::invalid_argument("dirac can only be used with discrete labels.");
    }
  }
  if(name == "linear"){
    return new Linear(type);
  }
  /*
  if(name == "gaussian") {
    return new Gaussian(type, sigma);
  }
  */
  throw std::invalid_argument("Unknown kernel specified: " + name);
}

NodeKernel *KernelFactory::getKernel(string name, string type){
  return getKernel(name, getLabelType(type));
}
LabelType KernelFactory::getLabelType(string type){
  if(type == "discrete"){
    return LabelType::Discrete;
  }
  if(type == "vector"){
    return LabelType::Vector;
  }
  throw std::invalid_argument(type + " does not name a proper label type. Options are discrete and vector");
}
