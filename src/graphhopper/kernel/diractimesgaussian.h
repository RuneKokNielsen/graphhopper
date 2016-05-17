#ifndef DIRACTIMESGAUSSIAN_H
#define DIRACTIMESGAUSSIAN_H

#include "nodekernel.h"
#include "labeltype.h"

#include <vector>

class DiracTimesGaussian: public NodeKernel{

 private:
  double computeVector(Node *v1, Node *v2, double mu);
  double computeDiscrete(Node *v1, Node *v2, double mu);

  double (DiracTimesGaussian::*_fCompute)(Node *v1, Node *v2, double mu);

 public:
  DiracTimesGaussian(LabelType labelType, double mu);
  double compute(Node *v1, Node *v2, double mu);
};

#endif
