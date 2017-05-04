#include "reflection.h"

namespace pbrt {

Spectrum ScaledBxDF::f(const Vector3f& wo, const Vector3f& wi) const {
  return scale*bxdf->f(wo, wi);
}

Spectrum ScaledBxDF::Sample_f(const Vector3f& wo, Vector3f* wi,
    const Point2f& sample, Float* pdf, BxDFType* sampledType) const {
  Spectrum f = bxdf->Sample_f(wo, wi, sample, pdf, sampledType);
  return scale*f;
}

Float FrDielectric(Float cosThetaI, Float etaI, Float etaT) {
  cosThetaI = Clamp(cosThetaI, -1, 1);

  // <potentially swap indices of refraction>
  bool entering = cosThetaI > 0.f;
  if (!entering) {
    std::swap(etaI, etaT);
    cosThetaI = std::abs(cosThetaI);
  }

  // <compute cosThetaI using Snell's law>
  Float sinThetaI = std::sqrt(std::max((Float)0, 1 - cosThetaI*cosThetaI));
  Float sinThetaT = etaI/etaT*sinThetaI;

  // <handle total internal reflection>
  if (sinThetaT >= 1) {
    return 1;
  }

  Float cosThetaT = std::sqrt(std::max((Float)0, 1 - sinThetaT*sinThetaT));

  Float Rparl = ((etaT*cosThetaI) - (etaI*cosThetaT))/((etaT*cosThetaI) + (etaI*cosThetaT));
  Float Rperp = ((etaI*cosThetaI) - (etaT*cosThetaT))/((etaI*cosThetaI) + (etaT*cosThetaT));

  return (Rparl*Rparl + Rperp*Rperp)/2;
}

Spectrum FrConductor(Float cosThetaI, const Spectrum& etaI,
    const Spectrum& etaT, const Spectrum& k) {

  cosThetaI = Clamp(cosThetaI, -1, 1);
  Spectrum eta = etaT/etaI;
  Spectrum etaK = k/etaI;

  Float cosThetaI2 = cosThetaI*cosThetaI;
  Float sinThetaI2 = 1. - cosThetaI2;
  Spectrum eta2 = eta*eta;
  Spectrum etaK2 = etaK*etaK;

  Spectrum t0 = eta2 - etaK2 - sinThetaI2;
  Spectrum a2plusb2 = Sqrt(t0*t0 + 4*eta2*etaK2);
  Spectrum t1 = a2plusb2 + cosThetaI2;
  Spectrum a = Sqrt(0.5f*(a2plusb2 + t0));
  Spectrum t2 = (Float)2*cosThetaI*a;
  Spectrum Rs = (t1 - t2)/(t1 + t2);

  Spectrum t3 = cosThetaI2*a2plusb2 + sinThetaI2*sinThetaI2;
  Spectrum t4 = t2*sinThetaI2;
  Spectrum Rp = Rs*(t3 - t4)/(t3 + t4);

  return 0.5*(Rp + Rs);
}

Spectrum FresnelConductor::Evaluate(Float cosThetaI) const {
  return FrConductor(std::abs(cosThetaI), etaI, etaT, k);
}

Spectrum FresnelDielectric::Evaluate(Float cosThetaI) const {
  return FrDielectric(cosThetaI, etaI, etaT);
}

Spectrum SpecularReflection::Sample_f(const Vector3f& wo, Vector3f* wi,
      const Point2f& sample, Float* pdf, BxDFType* sampledType) const {
  // <compute perfect specular reflection direction>
  *wi = Vector3f(-wo.x, -wo.y, wo.z);

  *pdf = 1;
  return fresnel->Evaluate(CosTheta(*wi))*R/AbsCosTheta(*wi);
}

Spectrum LambertianReflection::f(const Vector3f& wo, const Vector3f& wi) const {
  return R*InvPi;
}

} // namespace pbrt
