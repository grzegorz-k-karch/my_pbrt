#include "integrator.h"
#include "geometry.h"
#include "parallel.h"
#include "film.h"
#include "sampler.h"
#include "memory.h"
#include "camera.h"

namespace pbrt {

void SamplerIntegrator::Render(const Scene& scene) {
    Preprocess(scene, *sampler);

    // <render image tiles in parallel>
    // <compute number of tiles, nTiles, to use for parallel rendering>
    Bounds2i sampleBounds = camera->film->GetSampleBounds();
    Vector2i sampleExtent = sampleBounds.Diagonal();
    const int tileSize = 16;
    Point2i nTiles((sampleExtent.x + tileSize - 1)/tileSize,
                   (sampleExtent.y + tileSize - 1)/tileSize);

    ParallelFor2D([&](Point2i tile) {
        // <render section of image corresponding to tile>
        // <allocate memory arena for tile>
        MemoryArena arena;
        // <get sampler instance for tile>
        int seed = tile.y*nTiles.x + tile.x;
        std::unique_ptr<Sampler> tileSampler = sampler->Clone(seed);
        // <compute sample bounds for tile>
        int x0 = sampleBounds.pMin.x + tile.x*tileSize;
        int x1 = std::min(x0 + tileSize, sampleBounds.pMax.x);
        int y0 = sampleBounds.pMin.y + tile.y*tileSize;
        int y1 = std::min(y0 + tileSize, sampleBounds.pMax.y);
        Bounds2i tileBounds(Point2i(x0, y0), Point2i(x1, y1));
        // <get FilmTile for tile>
        std::unique_ptr<FilmTile> filmTile = camera->film->GetFilmTile(tileBounds);
        // <loop over pixel in tile to render them>
//        for (Point2i pixel : tileBounds) {
//        	tileSampler->StartPixel(pixel);
//        	do {
//        		// <initialize CameraSample for current sample>
//        		CameraSample cameraSample = tileSampler->GetCameraSample(pixel);
//
//        		// <generate camera ray for current sample>
//        		RayDifferential ray;
//        		Float rayWeight = camera->GenerateRayDifferential(cameraSample, &ray);
//        		ray.ScaleDifferential(1/std::sqrt(tileSampler->samplesPerPixel));
//
//        		// <evaluare radiance along camera ray>
//        		Spectrum L(0.0f);
//        		if (rayWeight > 0) {
//        			L = Li(ray, scene, *tileSampler, arena);
//        			// ... issue warning if unexpected radiance value is returned
//        		}
//
//        		// <add camera ray's contribution to image>
//        		filmTile->AddSample(cameraSample.pFilm, L, rayWeight);
//
//        		// <free MemoryArena memory from computing image sample value>
//        		arena.Reset();
//
//        	} while (tileSampler->StartNextSample());
//        }
        // ... merge image tile into Film
    }, nTiles);
    // ... save final image after rendering
}

}
