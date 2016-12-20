#include "film.h"
#include "geometry.h"

namespace pbrt {

Bounds2i Film::GetSampleBounds() const {
    return Bounds2i(Point2i(0,0), Point2i(512,512));
}

std::unique_ptr<FilmTile> Film::GetFilmTile(const Bounds2i& sampleBounds) {
	Vector2f halfPixel = Vector2f(0.5f,0.5f);
	Bounds2f floatBounds = Bounds2f(sampleBounds);
	Point2i p0;
	Point2i p1;

	Bounds2i tilePixelBounds = Intersect(Bounds2i(p0,p1), croppedPixelBounds);
}

}

