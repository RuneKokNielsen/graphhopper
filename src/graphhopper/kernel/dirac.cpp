#include "dirac.h"



double Dirac::compute(Node *v1, Node *v2){
  return v1->dLabel == v2->dLabel ? 1 : 0;
}
