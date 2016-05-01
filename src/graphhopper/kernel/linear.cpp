#include "linear.h"

Linear::Linear(LabelType labelType){
  switch(labelType){
  case Discrete:
    _fCompute = &Linear::computeDiscrete;
    break;
  case Vector:
    _fCompute = &Linear::computeVector;
    break;
  }
}

double Linear::compute(Node *v1, Node *v2){
  return (this->*_fCompute)(v1, v2);
}

double Linear::computeVector(Node *v1, Node *v2){
  double sum = 0;
  int sMin = min(v1->vLabel.size(), v2->vLabel.size());
  for(int i=0; i<sMin; i++){
    sum += v1->vLabel[i] * v2->vLabel[i];
  }
  return sum;
}

double Linear::computeDiscrete(Node *v1, Node *v2){
  return v1->dLabel * v2->dLabel;
}
