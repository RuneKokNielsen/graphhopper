#ifndef GAUSSIAN_H
#define GAUSSIAN_H

#include "nodekernel.h"
#include "labeltype.h"

#include <math.h>
#include <vector>
#include <algorithm>

class Gaussian: public NodeKernel{

 private:
  double computeVector(Node *v1, Node *v2);
  double computeDiscrete(Node *v1, Node *v2);

  double (Gaussian::*_fCompute)(Node *v1, Node *v2);

  double sigma;

 public:
  Gaussian(LabelType labelType, double sigma);
  double compute(Node *v1, Node *v2);
};

#endif
