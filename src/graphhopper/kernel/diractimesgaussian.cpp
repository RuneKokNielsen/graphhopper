
#include "diractimesgaussian"

DiracTimesGaussian::DiracTimesGaussian(LabelType labelType, double sigma) {
  switch(labelType) {
  case Discrete:
    _fCompute = &DiracTimesGaussian::computeDiscrete;
    break;
  case Vector:
    _fCompute = &DiracTimesGaussian::computeVector;
    break;
  }
}

double DiracTimesGaussian::compute(Node *v1, Node *v2, double sigma) {
  return (this->*fCompute)(v1, v2, sigma);
}

double DiracTimesGaussian::computeVector(Node *v1, Node *v2, double sigma) {
  /*
  vector<int> dirac(v1 -> vLabel.size());
  for(int i = 0; i < v1 -> vLabel.size(); i++) {
    dirac[i] = v1->vLabel == v2->vLabel ? 1 : 0;
  }
  */
  return 0.0;
}


double DiracTimesGaussian::computeDiscrete(Node *v1, Node *v2, double sigma) {

  return 0.0;
}
