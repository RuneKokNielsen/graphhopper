
#include "gaussian.h"

Gaussian::Gaussian(LabelType labelType, double mu) {
  this -> mu = mu;
  switch(labelType) {
  case Discrete:
    _fCompute = &Gaussian::computeDiscrete;
    break;
  case Vector:
    _fCompute = &Gaussian::computeVector;
    break;
  }
}

double Gaussian::compute(Node *v1, Node *v2) {
  return (this->*_fCompute)(v1, v2);
}

double Gaussian::computeVector(Node *v1, Node *v2) {

  double normxny = 0;
  for(int i=0; i<v1->vLabel.size(); i++){ //Assuming same vector length
    double d = v1->vLabel[i] - v2->vLabel[i];
    normxny += d * d;
  }

  return exp(-mu * normxny);
}

double Gaussian::computeDiscrete(Node *v1, Node *v2) {
  double normxny = v1 -> dLabel - v2 -> dLabel;
  //return exp(-(normxny*normxny)/(2.0 * pow(sigma, 2.0)));
  return exp(-mu * pow(normxny, 2.0));
}
