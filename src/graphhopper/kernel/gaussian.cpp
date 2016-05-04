
#include "gaussian.h"

Gaussian::Gaussian(LabelType labelType, double sigma) {
  switch(labelType) {
  case Discrete:
    _fCompute = &Gaussian::computeDiscrete;
    break;
  case Vector:
    _fCompute = &Gaussian::computeVector;
    break;
  }
}

double Gaussian::compute(Node *v1, Node *v2, double sigma) {
  return (this->*_fCompute)(v1, v2, sigma);
}

double Gaussian::computeVector(Node *v1, Node *v2, double sigma) {
  vector<double> xny(v1 -> vLabel.size());
  for(int i = 0; i < v1 -> vLabel.size(); i++) {
    xny[i] = v1 -> vLabel[i] - v2 -> vLabel[i];
  }
  double normxny = 0;
  for(int i = 0; i < v1 -> vLabel.size(); i++) {
    normxny += xny[i] * xny[i];
  }
  return exp(-normxny/(pow(2.0*sigma, 2.0)));
}

double Gaussian::computeDiscrete(Node *v1, Node *v2, double sigma) {
  double xny = v1 -> dLabel - v2 -> dLabel;
  return exp(-(xny*xny)/(pow(2.0*sigma, 2.0)));
}

