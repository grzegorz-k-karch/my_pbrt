#include "film.h"
#include "geometry.h"

namespace pbrt {

Film::Film(const Point2i& resolution, const Bounds2f& cropWindow,
      std::unique_ptr<Filter> filt, Float diagonal,
      const std::string& filename, Float scale)
: fullResolution(resolution), diagonal(diagonal*.001),
  filter(std::move(filt)), filename(filename), scale(scale) {

  // <compute film image bounds>
  croppedPixelBounds =
      Bounds2i(Point2i(std::ceil(fullResolution.x*cropWindow.pMin.x),
          std::ceil(fullResolution.y*cropWindow.pMin.y)),
          Point2i(std::ceil(fullResolution.x*cropWindow.pMax.x),
              std::ceil(fullResolution.y*cropWindow.pMax.y)));

  // <allocate film image storage>
  pixels = std::unique_ptr<Pixel[]>(new Pixel[croppedPixelBounds.Area()]);

  // <precompute filter weight table>
  int offset = 0;
  for (int y = 0; y < filterTableWidth; ++y) {
    for (int x = 0; x < filterTableWidth; ++x) {
      Point2f p;
      p.x = (x + 0.5f)*filter->radius.x / filterTableWidth;
      p.y = (y + 0.5f)*filter->radius.y / filterTableWidth;
      filterTable[offset] = filter->Evaluate(p);
    }
  }
}

Bounds2i Film::GetSampleBounds() const {
  Bounds2f floatBounds(
      Floor(Point2f(croppedPixelBounds.pMin) + Vector2f(0.5f, 0.5f) - filter->radius),
      Ceil(Point2f(croppedPixelBounds.pMax) - Vector2f(0.5f, 0.5f) + filter->radius));
    return (Bounds2i)floatBounds;
}

std::unique_ptr<FilmTile> Film::GetFilmTile(const Bounds2i& sampleBounds) {

  // <bound image pixels that samples in sampleBounds contribute to>
  Vector2f halfPixel = Vector2f(0.5f,0.5f);
  Bounds2f floatBounds = Bounds2f(sampleBounds);
  Point2i p0 = (Point2i)Ceil(floatBounds.pMin - halfPixel - filter->radius);
  Point2i p1 = (Point2i)Floor(floatBounds.pMax - halfPixel + filter->radius) + Point2i(1,1);
  Bounds2i tilePixelBounds = Intersect(Bounds2i(p0,p1), croppedPixelBounds);

  return std::unique_ptr<FilmTile>(new FilmTile(tilePixelBounds,
      filter->radius, filterTable, filterTableWidth));
}

Bounds2f Film::GePhysicalExtent() const {

  Float aspect = (Float)fullResolution.y/(Float)fullResolution.x;
  Float x = std::sqrt(diagonal*diagonal/(1 + aspect*aspect));
  Float y = aspect*x;
  return Bounds2f(Point2f(-x/2, -y/2), Point2f(x/2, y/2));
}

void Film::MergeFilmTile(std::unique_ptr<FilmTile> tile) {

  std::lock_guard<std::mutex> lock(mutex);
  for (Point2i pixel : tile->GetPixelBounds()) {
    // <merge tile into Film::pixels>
    const FilmTilePixel &tilePixel = tile->GetPixel(pixel);
    Pixel &mergePixel = GetPixel(pixel);
    Float xyz[3];
    tilePixel.contribSum.ToXYZ(xyz);
    for (int i = 0; i < 3; ++i) {
      mergePixel.xyz[i] += xyz[i];
    }
    mergePixel.filterWeigthSum += tilePixel.filterWeightSum;
  }
}

void Film::SetImage(const Spectrum* img) const {

  int nPixels = croppedPixelBounds.Area();
  for (int i = 0; i < nPixels; ++i) {
    Pixel &p = pixels[i];
    img[i].ToXYZ(p.xyz);
    p.filterWeigthSum  = 1;
    p.splatXYZ[0] = p.splatXYZ[1] = p.splatXYZ[2] = 0;
  }
}

void Film::AddSplat(const Point2f& p, const Spectrum& v) {

  if (!InsideExclusive((Point2i)p, croppedPixelBounds))
    return;
  Float xyz[3];
  v.ToXYZ(xyz);
  Pixel &pixel = GetPixel((Point2i)p);
  for (int i = 0; i < 3; ++i) {
    pixel.splatXYZ[i].Add(xyz[i]);
  }
}

void Film::WriteImage(Float splatScale) {

  // <convert image to RGB and compute final pixel values>
  std::unique_ptr<Float[]> rgb(new Float[3*croppedPixelBounds.Area()]);
  int offset = 0;

  for (Point2i p : croppedPixelBounds) {
    // <convert pixel XYZ color to RGB>
    Pixel &pixel = GetPixel(p);
    XYZToRGB(pixel.xyz, &rgb[3*offset]);

    // <normalize pixel with weight sum>
    Float filterWeightSum = pixel.filterWeigthSum;
    if (filterWeightSum != 0) {
      Float invWt = (Float)1/filterWeightSum;
      rgb[3*offset  ] = std::max((Float)0, rgb[3*offset  ]*invWt);
      rgb[3*offset+1] = std::max((Float)0, rgb[3*offset+1]*invWt);
      rgb[3*offset+2] = std::max((Float)0, rgb[3*offset+2]*invWt);
    }

    // <add splat value at pixel>
    Float splatRGB[3];
    Float splatXYZ[3] = {pixel.splatXYZ[0], pixel.splatXYZ[1], pixel.splatXYZ[2]};
    XYZToRGB(splatXYZ, splatRGB);
    rgb[3*offset  ] += splatScale*splatRGB[0];
    rgb[3*offset+1] += splatScale*splatRGB[1];
    rgb[3*offset+2] += splatScale*splatRGB[2];

    // <scale pixel value by scale>
    rgb[3*offset  ] *= scale;
    rgb[3*offset+1] *= scale;
    rgb[3*offset+2] *= scale;

    ++offset;
  }

  // <write RGB image>
  // TODO ::WriteImage(filename, &rgb[0], croppedPixelBounds, fullResolution);
}

}

