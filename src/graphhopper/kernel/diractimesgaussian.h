#ifndef DIRACTIMESGAUSSIAN_H
#define DIRACTIMESGAUSSIAN_H

#include "nodekernel.h"
#include "labeltype.h"

#include <vector>

class DiracTimesGaussian: public NodeKernel{

 private:
  double computeVector(Node *v1, Node *v2, double sigma);
  double computeDiscrete(Node *v1, Node *v2, double sigma);

  double (DiracTimesGaussian::*_fCompute)(Node *v1, Node *v2, double sigma);

 public:
  DiracTimesGaussian(LabelType labelType, double sigma);
  double compute(Node *v1, Node *v2, double sigma);
};

#endif
