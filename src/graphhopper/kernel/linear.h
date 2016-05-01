#ifndef LINEAR_H
#define LINEAR_H

#include "nodekernel.h"
#include "labeltype.h"

class Linear: public NodeKernel{

 private:
  double computeVector(Node *v1, Node *v2);
  double computeDiscrete(Node *v1, Node *v2);

  double (Linear::*_fCompute)(Node *v1, Node *v2);

 public:

  Linear(LabelType labelType);

  double compute(Node *v1, Node *v2);

};


#endif
