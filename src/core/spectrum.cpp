#include "spectrum.h"

namespace pbrt {

void Blackbody(const Float* lambda, int n, Float T, Float* Le) {

  const Float c = 299792458;
  const Float h = 6.62606957e-34;
  const Float kb = 1.3806488e-23;

  for (int i = 0; i < n; ++i) {
    // <compute emitted radiance for blackbody at wavelength lambda[i]>
    Float l = lambda[i]*1e-9;
    Float lambda5 = (l*l)*(l*l)*l;
    Le[i] = (2*h*c*c)/(lambda5*(std::exp((h*c)/(l*kb*T))-1));
  }
}

void BlackbodyNormalized(const Float* lambda, int n, Float T, Float* Le) {

  Blackbody(lambda, n, T, Le);

  // <normalize Le values based on maximum blackbody radiance>
  Float lambdaMax = 2.8977721e-3/T*1e9;
  Float maxL;
  Blackbody(&lambdaMax, 1, T, &maxL);
  for (int i = 0; i < n; ++i) {
    Le[i] /= maxL;
  }
}

} // namespace pbrt
