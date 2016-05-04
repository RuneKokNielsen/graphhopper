
#include "bridge.h"

Bridge::Bridge(LabelType labelType) {
  switch(labelType) {
  case Discrete:
    _fCompute = &Bridge::computeDiscrete;
    break;
  case Vector:
    _fCompute = &Bridge::computeVector;
    break;
  }
}

double compute(Node *v1, Node *v2) {
  return (this->*_fCompute)(v1, v2);
}

double computeVector(Node *v1, Node *v2) {
  int c = 5;
  return 0.0;//std::max(0, c - std::abs());
}

double computeDiscrete(Node *v1, Node *v2) {
  return 0.0;
}
