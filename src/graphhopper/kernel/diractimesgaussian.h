#ifndef DIRACTIMESGAUSSIAN_H
#define DIRACTIMESGAUSSIAN_H

#include "nodekernel.h"
#include "labeltype.h"

#include <vector>

class DiracTimesGaussian: public NodeKernel{

 private:

  double _mu;

 public:
  DiracTimesGaussian(double mu);
  double compute(Node *v1, Node *v2);
};

#endif
