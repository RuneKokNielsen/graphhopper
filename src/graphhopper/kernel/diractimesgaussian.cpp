
#include "diractimesgaussian.h"

DiracTimesGaussian::DiracTimesGaussian(double mu) {
  _mu = mu;
}


double DiracTimesGaussian::compute(Node *v1, Node *v2) {

  if(v1->dLabel != v2->dLabel) return 0;

  double normxny = 0;
  for(int i=0; i<v1->vLabel.size(); i++){ //Assuming same vector length
    double d = v1->vLabel[i] - v2->vLabel[i];
    normxny += d * d;
  }
  return exp(-this->_mu * normxny);
}
