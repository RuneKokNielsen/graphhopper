#include "kernelfactory.h"
#include "dirac.cpp"
#include "linear.cpp"
#include "gaussian.cpp"
#include "bridge.cpp"

NodeKernel *KernelFactory::getKernel(string name, LabelType type, double sigma){
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
  if(name == "gaussian") {
    return new Gaussian(type, sigma);
  }
  if(name == "bridge") {
    return new Bridge(type, sigma);
  }
  throw std::invalid_argument("Unknown kernel specified: " + name);
}

NodeKernel *KernelFactory::getKernel(string name, string type, double sigma){
  return getKernel(name, getLabelType(type), sigma);
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
