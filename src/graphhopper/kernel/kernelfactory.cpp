#include "kernelfactory.h"
#include "dirac.cpp"
#include "linear.cpp"
#include "gaussian.cpp"
#include "bridge.cpp"
#include "diractimesgaussian.cpp"
#include <stdexcept>

#ifdef MEX
#include "mex.h"
#endif

NodeKernel *KernelFactory::getKernel(string name, LabelType type, double kernelParameter){
  if(name == "dirac"){
    if(type == Discrete){
      return new Dirac();
    }else{
      mexPrintf("dirac can only be used with discrete labels.\n");
      throw std::invalid_argument("dirac can only be used with discrete labels.");
    }
  }
  if(name == "linear"){
    if(type == Both){
      mexPrintf("linear can not be used with both node labels\n");
      throw std::invalid_argument("linear can not be used with both node labels\n");
    }
    return new Linear(type);
  }
  if(name == "gaussian") {
    if(type == Both){
      mexPrintf("gaussian can not be used with both node labels\n");
      throw std::invalid_argument("gaussian can not be used with both node labels\n");
    }
    return new Gaussian(type, kernelParameter);
  }
  if(name == "bridge") {
    if(type == Both){
      mexPrintf("bridge can not be used with both node labels\n");
      throw std::invalid_argument("bridge can not be used with both node labels\n");
    }
    return new Bridge(type, kernelParameter);
  }

  if(name == "diracxgaussian"){
    if(type == Both){
      return new DiracTimesGaussian(kernelParameter);
    }else{
      mexPrintf("diracxgaussian can only be used with both discrete and vector labels (use command 'both')\n");
      throw std::invalid_argument("diracxgaussian must be used with both discrete and vector labels.");
    }
  }

  throw std::invalid_argument("Unknown kernel specified: " + name);
}

NodeKernel *KernelFactory::getKernel(string name, string type, double kernelParameter){
  return getKernel(name, getLabelType(type), kernelParameter);
}
LabelType KernelFactory::getLabelType(string type){
  if(type == "discrete"){
    return LabelType::Discrete;
  }
  if(type == "vector"){
    return LabelType::Vector;
  }
  if(type == "both"){
    return LabelType::Both;
  }
  mexPrintf("Unknown label type: %s\n", type.c_str());
  throw std::invalid_argument(type + " does not name a proper label type. Options are discrete and vector");
}
