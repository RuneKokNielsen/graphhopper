#ifndef BRIDGE_H
#define BRIDGE_H

#include "nodekernel.h"
#include "labeltype.h"

#include <algorithm>
#include <cmath>

class Bridge: public NodeKernel{

 private:
  double computeVector(Node *v1, Node *v2);
  double computeDiscrete(Node *v1, Node *v2);

  double (Bridge::*_fCompute)(Node *v1, Node *v2);

 public:
  Bridge(LabelType labelType, double sigma);
  double compute(Node *v1, Node *v2, double sigma);
};

#endif
