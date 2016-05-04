#ifndef GAUSSIAN_H
#define GAUSSIAN_H

#include "nodekernel.h"
#include "labeltype.h"

#include <math.h>
#include <vector>

class Gaussian: public NodeKernel{

 private:
  double computeVector(Node *v1, Node *v2, double sigma);
  double computeDiscrete(Node *v1, Node *v2, double sigma);

  double (Gaussian::*_fCompute)(Node *v1, Node *v2, double sigma);

 public:
  Gaussian(LabelType labelType, double sigma);
  double compute(Node *v1, Node *v2, double sigma);
};

#endif
