#ifndef SHAPES_TRIANGLE_H
#define SHAPES_TRIANGLE_H

#include <memory>

#include "transform.h"
#include "texture.h"
#include "geometry.h"
#include "shape.h"

namespace pbrt {

struct TriangleMesh {

  // <triangle mesh public methods>
  TriangleMesh(const Transform& objectToWorld, int _nTriangles, const int* _vertexIndices,
      int _nVertices, const Point3f* P, const Vector3f* S, const Normal3f* N,
      const Point2f* UV, const std::shared_ptr<Texture<Float>>& _alphaMask);

  // <triangle mesh data>
  const int nTriangles, nVertices;
  std::vector<int> vertexIndices;
  std::unique_ptr<Point3f[]> p;
  std::unique_ptr<Normal3f[]> n;
  std::unique_ptr<Vector3f[]> s;
  std::unique_ptr<Point2f[]> uv;
  std::shared_ptr<Texture<Float>> alphaMask;
};

class Triangle : public Shape {
public:
  // <public methods>
  Triangle(const Transform* objectToWorld, const Transform* worldToObject, bool reverseOrientation,
      const std::shared_ptr<TriangleMesh>& mesh, int triNumber)
: Shape(objectToWorld, worldToObject, reverseOrientation), mesh(mesh) {
    v = &mesh->vertexIndices[3*triNumber];
  }

  std::vector<std::shared_ptr<Shape>>
  CreateTraingleMesh(const Transform* objectToWorld, const Transform* worldToObject,
                     bool reverseOrientation, int nTriangles, const int* vertexIndices,
                     int nVertices, const Point3f* p, const Vector3f* s, const Normal3f* n,
                     const Point2f* uv, const std::shared_ptr<Texture<Float>>& alphaMask);

  virtual Bounds3f ObjectBound() const override;
  virtual Bounds3f WorldBound() const override;

  virtual bool Intersect(const Ray& ray, Float* tHit, SurfaceInteraction* isect,
                         bool testAlphaTexture = true) const override;

  virtual Float Area() const override;

private:
  // <private methods>
  void GetUVs(Point2f uv[3]) const {
    if (mesh->uv) {
      uv[0] = mesh->uv[v[0]];
      uv[1] = mesh->uv[v[1]];
      uv[2] = mesh->uv[v[2]];
    }
    else {
      uv[0] = Point2f(0, 0);
      uv[1] = Point2f(1, 0);
      uv[2] = Point2f(1, 1);
    }
  }

  // <private data>
  std::shared_ptr<TriangleMesh> mesh;
  const int *v;
};

} // namespace pbrt

#endif//SHAPES_TRIANGLE_H
