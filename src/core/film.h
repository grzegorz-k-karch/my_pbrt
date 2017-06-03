#ifndef FILM_H
#define FILM_H

#include "geometry.h"
#include "filter.h"
#include "pbrt.h"
#include "parallel.h"
#include "spectrum.h"
#include <vector>
#include <memory>
#include <mutex>

namespace pbrt {

struct FilmTilePixel {
  Spectrum contribSum = 0.0f;
	Float filterWeightSum = 0.0f;
};

class FilmTile {

public:
	FilmTile(const Bounds2i& pixelBounds, const Vector2f& filterRadius,
			const Float* filterTable, int filterTableSize)
	: pixelBounds(pixelBounds), filterRadius(filterRadius),
	  invFilterRadius(1/filterRadius.x, 1/filterRadius.y),
	  filterTable(filterTable), filterTableSize(filterTableSize) {
		pixels = std::vector<FilmTilePixel>(std::max(0,pixelBounds.Area()));
	}

	void AddSample(const Point2f& pFilm, const Spectrum& L, Float sampleWeight = 1.f) {
	  // <compute sample's raster bounds>
	  Point2f pFilmDiscrete = pFilm - Vector2f(0.5f, 0.5f);
	  Point2i p0 = (Point2i)Ceil(pFilmDiscrete - filterRadius);
	  Point2i p1 = (Point2i)Floor(pFilmDiscrete + filterRadius) + Point2i(1, 1);
	  p0 = Max(p0, pixelBounds.pMin);
	  p1 = Min(p1, pixelBounds.pMax);

	  // <loop over filter support and add sample to pixel arrays>
	  // <precompute x and y filter table offsets>
	  int *ifx = ALLOCA(int, p1.x - p0.x);
	  for (int x = p0.x; x < p1.x; ++x) {
	    Float fx = std::abs((x - pFilmDiscrete.x)*invFilterRadius.x*filterTableSize);
	    ifx[x - p0.x] = std::min((int)std::floor(fx), filterTableSize - 1);
	  }
	  int *ify = ALLOCA(int, p1.y - p0.y);
	  for (int y = p0.y; y < p1.y; ++y) {
	    Float fy = std::abs((y - pFilmDiscrete.y)*invFilterRadius.y*filterTableSize);
	    ify[y - p0.y] = std::min((int)std::floor(fy), filterTableSize -1);
	  }

	  for (int y = p0.y; y < p1.y; ++y) {
	    for (int x = p0.x; x < p1.x; ++x) {
	      // <evaluate filter value at (x,y) pixel>
	      int offset = ify[y - p0.y]*filterTableSize + ifx[x - p0.x];
	      Float filterWeight = filterTable[offset];

	      // <update pixel values with filtered sample contribution>
	      FilmTilePixel &pixel = GetPixel(Point2i(x, y));
	      pixel.contribSum += L*sampleWeight*filterWeight;
	      pixel.filterWeightSum += filterWeight;
	    }
	  }
	}

	FilmTilePixel& GetPixel(const Point2i& p) {

	  int width = pixelBounds.pMax.x - pixelBounds.pMin.x;
	  int offset = (p.x - pixelBounds.pMin.x) + (p.y - pixelBounds.pMin.y)*width;
	  return pixels[offset];
	}

	Bounds2i GetPixelBounds() const { return pixelBounds; }
private:
	const Bounds2i pixelBounds;
	const Vector2f filterRadius, invFilterRadius;
	const Float *filterTable;
	const int filterTableSize;
	std::vector<FilmTilePixel> pixels;
};

class Film {

public:
  Film(const Point2i& resolution, const Bounds2f& cropWindow,
      std::unique_ptr<Filter> filt, Float diagonal,
      const std::string& filename, Float scale);
	Bounds2i GetSampleBounds() const;
	std::unique_ptr<FilmTile> GetFilmTile(const Bounds2i& sampleBounds);
	Bounds2f GePhysicalExtent() const;
	void MergeFilmTile(std::unique_ptr<FilmTile> tile);
	void SetImage(const Spectrum* img) const;
	void AddSplat(const Point2f& p, const Spectrum& v);

	void WriteImage(Float splatScale=1);

	const Point2i fullResolution;
	const Float diagonal;
	std::unique_ptr<Filter> filter;
	const std::string filename;
	Bounds2i croppedPixelBounds; // <here

private:
	struct Pixel {
	  Float xyz[3] = {0, 0, 0};
	  Float filterWeigthSum = 0;
	  AtomicFloat splatXYZ[3];
	  Float pad;
	};
	std::unique_ptr<Pixel[]> pixels;
	const Float scale;
	static constexpr int filterTableWidth = 16;
	Float filterTable[filterTableWidth*filterTableWidth];
	std::mutex mutex;

	Pixel& GetPixel(const Point2i& p) const {
    int width = croppedPixelBounds.pMax.x - croppedPixelBounds.pMin.x;
    int offset = (p.x - croppedPixelBounds.pMin.x) +
        (p.y - croppedPixelBounds.pMin.y)*width;
    return pixels[offset];
  }
};

}

#endif//FILM_H
