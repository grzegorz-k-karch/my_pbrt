#ifndef FILM_H
#define FILM_H

#include "geometry.h"
#include "pbrt.h"
#include "film.h"
#include <vector>
#include <memory>

namespace pbrt {

struct FilmTilePixel {
//	Spectrum contribSum = 0.0f; // TODO
	Float filterWeightSum = 0.0f;
};

class FilmTile {

public:
	FilmTile(const Bounds2i& pixelBounds, const Vector2i& filterRadius,
			const Float* filterTable, int filterTableSize)
	: pixelBounds(pixelBounds), filterRadius(filterRadius),
	  invFilterRadius(1/filterRadius.x, 1/filterRadius.y),
	  filterTable(filterTable), filterTableSize(filterTableSize) {
		pixels = std::vector<FilmTilePixel>(std::max(0,pixelBounds.Area()));
	}
private:
	const Bounds2i pixelBounds;
	const Vector2i filterRadius, invFilterRadius;
	const Float *filterTable;
	const int filterTableSize;
	std::vector<FilmTilePixel> pixels;
};

class Film {

public:
	Bounds2i GetSampleBounds() const;
	std::unique_ptr<FilmTile> GetFilmTile(const Bounds2i& sampleBounds);
	Bounds2i croppedPixelBounds; // <here
};

}

#endif//FILM_H
