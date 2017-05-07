#ifndef CORE_REFLECTION_H
#define CORE_REFLECTION_H

#include "pbrt.h"
#include "geometry.h"
#include "spectrum.h"
#include "interaction.h"

#include <algorithm>

namespace pbrt {

inline Float CosTheta(const Vector3f& w) {
  return w.z;
}
inline Float Cos2Theta(const Vector3f& w) {
  return w.z*w.z;
}
inline Float AbsCosTheta(const Vector3f& w) {
  return std::abs(w.z);
}
inline Float Sin2Theta(const Vector3f& w) {
  return std::max((Float)0, (Float)1 - Cos2Theta(w));
}
inline Float SinTheta(const Vector3f& w) {
  return std::sqrt(Sin2Theta(w));
}
inline Float TanTheta(const Vector3f& w) {
  return CosTheta(w)/SinTheta(w);
}
inline Float Tan2Theta(const Vector3f& w) {
  return Cos2Theta(w)/Sin2Theta(w);
}
inline Float CosPhi(const Vector3f& w) {
  Float sinTheta = SinTheta(w);
  return (sinTheta == 0) ? 1 : Clamp(w.x/sinTheta, -1, 1);
}
inline Float SinPhi(const Vector3f& w) {
  Float sinTheta = SinTheta(w);
  return (sinTheta == 0) ? 1 : Clamp(w.y/sinTheta, -1, 1);
}
inline Float Cos2Phi(const Vector3f& w) {
  return CosPhi(w)*CosPhi(w);
}
inline Float Sin2Phi(const Vector3f& w) {
  return SinPhi(w)*SinPhi(w);
}
inline Float CosDPhi(const Vector3f& wa, const Vector3f& wb) {
  return Clamp((wa.x*wb.x + wa.y*wb.y)/
      std::sqrt((wa.x*wa.x + wa.y*wa.y)*(wb.x*wb.x + wb.y*wb.y)), -1, 1);
}
inline Vector3f Reflect(const Vector3f& wo, const Vector3f& n) {
  return -wo + 2*Dot(wo,n)*n;
}
inline bool SameHemisphere(const Vector3f& w, const Vector3f& wp) {
  return w.z*wp.z > 0;
}

enum BxDFType {
  BSDF_REFLECTION = 1 << 0,
  BSDF_TRANSMISSION = 1 << 1,
  BSDF_DIFFUSE = 1 << 2,
  BSDF_GLOSSY = 1 << 3,
  BSDF_SPECULAR = 1 << 4,
  BSDF_ALL = BSDF_DIFFUSE | BSDF_GLOSSY | BSDF_SPECULAR | BSDF_REFLECTION | BSDF_TRANSMISSION
};

class BxDF {
public:
  BxDF(BxDFType type) : type(type) {}

  bool MatchesFlags(BxDFType t) const {
    return (type & t) == type;
  }

  virtual Spectrum f(const Vector3f& wo, const Vector3f& wi) const = 0;
  virtual Spectrum Sample_f(const Vector3f& wo, Vector3f* wi,
      const Point2f& sample, Float* pdf, BxDFType* sampledType = nullptr) const;
  virtual Spectrum rho(const Vector3f& w, int nSamples, const Point2f* samples) const;
  virtual Spectrum rho(int nSamples, const Point2f* samples1, const Point2f* samples2) const;
  Float Pdf(const Vector3f& wo, const Vector3f& wi) const;

  const BxDFType type;
};

class ScaledBxDF : public BxDF {
public:
  ScaledBxDF(BxDF* bxdf, const Spectrum scale)
: BxDF(BxDFType(bxdf->type)), bxdf(bxdf), scale(scale) {}

  Spectrum f(const Vector3f& wo, const Vector3f& wi) const override;
  Spectrum Sample_f(const Vector3f& wo, Vector3f* wi,
      const Point2f& sample, Float* pdf, BxDFType* sampledType = nullptr) const override;

  Spectrum rho(const Vector3f& w, int nSamples, const Point2f* samples) const override {
    return scale*bxdf->rho(w, nSamples, samples);
  }
  Spectrum rho(int nSamples, const Point2f* samples1, const Point2f* samples2) const override {
    return scale*bxdf->rho(nSamples, samples1, samples2);
  }

private:
  BxDF *bxdf;
  Spectrum scale;
};

class Fresnel {
public:
  virtual Spectrum Evaluate(Float cosI) const = 0;
};

class FresnelConductor : public Fresnel {
public:
  FresnelConductor(const Spectrum& etaI, const Spectrum& etaT, const Spectrum& k) :
    etaI(etaI), etaT(etaT), k(k) {}
  Spectrum Evaluate(Float cosThetaI) const override;
private:
  Spectrum etaI, etaT, k;
};

class FresnelDielectric : public Fresnel {
public:
  FresnelDielectric(Float etaI, Float etaT) : etaI(etaI), etaT(etaT) {}
  Spectrum Evaluate(Float cosThetaI) const override;
private:
  Float etaI, etaT;
};

class FresnelNoOp : public Fresnel {
public:
  Spectrum Evaluate(Float) const { return Spectrum(1.); }
};

class SpecularReflection : public BxDF {
public:
  SpecularReflection(const Spectrum& R, Fresnel* fresnel)
: BxDF(BxDFType(BSDF_REFLECTION | BSDF_SPECULAR)), R(R), fresnel(fresnel) {}

  Spectrum f(const Vector3f& wo, const Vector3f& wi) const override {
    return Spectrum(0.f);
  }
  Spectrum Sample_f(const Vector3f& wo, Vector3f* wi,
      const Point2f& sample, Float* pdf, BxDFType* sampledType) const override;
private:
  const Spectrum R;
  const Fresnel *fresnel;
};

class LambertianReflection : public BxDF {
public:
  LambertianReflection(const Spectrum& R)
: BxDF(BxDFType(BSDF_REFLECTION | BSDF_DIFFUSE)), R(R) {}
  Spectrum f(const Vector3f& wo, const Vector3f& wi) const override;

  Spectrum rho(const Vector3f& w, int nSamples, const Point2f* samples) const override {
    return R;
  }
  Spectrum rho(int nSamples, const Point2f* samples1, const Point2f* samples2) const override {
    return R;
  }
private:
  const Spectrum R;
};

class BSDF {
public:
  BSDF(const SurfaceInteraction& si, Float eta = 1)
: eta(eta), ns(si.shading.n), ng(si.n), ss(Normalize(si.shading.dpdu)),
  ts(Cross(ns,ss)) {}

  void Add(BxDF* b) {
    Assert(nBxDFs < MaxBxDFs);
    bxdfs[nBxDFs++] = b;
  }

  int NumComponents(BxDFType flags = BSDF_ALL) const;

  Vector3f WorldToLocal(const Vector3f& v) const {
    return Vector3f(Dot(v, ss), Dot(v, ts), Dot(v, ns));
  }

  Vector3f LocalToWorld(Vector3f& v) const {
    return Vector3f(ss.x*v.x + ts.x*v.y + ns.x*v.z,
    ss.y*v.x + ts.y*v.y + ns.y*v.z,
    ss.z*v.x + ts.z*v.y + ns.z*v.z);
  }

  Spectrum f(const Vector3f& woW, const Vector3f& wiW, BxDFType flags) const;
  Spectrum rho(int nSamples, const Point2f* samples1,
      const Point2f* samples2, BxDFType flags = BSDF_ALL) const;
  Spectrum rho(const Vector3f& wo, int nSamples,
      const Point2f* samples, BxDFType flags = BSDF_ALL) const;
  const Float eta;
private:
  ~BSDF() {}
  const Normal3f ns, ng;
  const Vector3f ss, ts;
  int nBxDFs = 0;
  static constexpr int MaxBxDFs = 8;
  BxDF *bxdfs[MaxBxDFs];
};

Float FrDielectric(Float cosThetaI, Float etaI, Float etaT);
Spectrum FrConductor(Float cosThetaI, const Spectrum& etaI,
    const Spectrum& etaT, const Spectrum& k);


} // namespace pbrt


#endif // CORE_REFLECTION_H
