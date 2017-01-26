all: pbrt

pbrt: material.o primitive.o pbrt.o sphere.o efloat.o triangle.o texture.o scene.o integrator.o parallel.o film.o memory.o shape.o sampler.o whitted.o light.o transform.o error.o interaction.o
	g++ $^ -o $@

pbrt.o: main/pbrt.cpp
	g++ -std=c++11 -c $^

sphere.o: shapes/sphere.cpp shapes/sphere.h
	g++ -std=c++11 -c $< -Icore

triangle.o: shapes/triangle.cpp shapes/triangle.h
	g++ -std=c++11 -c $< -Icore

scene.o: core/scene.cpp core/scene.h
	g++ -std=c++11 -c $<

material.o: core/material.cpp core/material.h
	g++ -std=c++11 -c $<

primitive.o: core/primitive.cpp core/primitive.h
	g++ -std=c++11 -c $<

texture.o: core/texture.cpp core/texture.h
	g++ -std=c++11 -c $<

integrator.o: core/integrator.cpp core/integrator.h
	g++ -std=c++11 -c $<

parallel.o: core/parallel.cpp core/parallel.h
	g++ -std=c++11 -c $<

film.o: core/film.cpp core/film.h
	g++ -std=c++11 -c $<

memory.o: core/memory.cpp core/memory.h
	g++ -std=c++11 -c $<

efloat.o: core/efloat.cpp core/efloat.h
	g++ -std=c++11 -c $<

shape.o: core/shape.cpp core/shape.h
	g++ -std=c++11 -c $<

sampler.o: core/sampler.cpp core/sampler.h
	g++ -std=c++11 -c $<

light.o: core/light.cpp core/light.h
	g++ -std=c++11 -c $<

transform.o: core/transform.cpp core/transform.h
	g++ -std=c++11 -c $<

error.o: core/error.cpp core/error.h
	g++ -std=c++11 -c $<

interaction.o: core/interaction.cpp core/interaction.h
	g++ -std=c++11 -c $<

whitted.o: integrators/whitted.cpp integrators/whitted.h
	g++ -std=c++11 -c $< -Icore

clean:
	rm -f ./*~ ./*.o pbrt
