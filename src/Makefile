all: pbrt

pbrt: pbrt.o scene.o integrator.o parallel.o film.o memory.o shape.o sampler.o whitted.o light.o transform.o error.o interaction.o
	g++ $^ -o $@

pbrt.o: main/pbrt.cpp
	g++ -std=c++11 -c $^

scene.o: core/scene.cpp core/scene.h
	g++ -std=c++11 -c $<

integrator.o: core/integrator.cpp core/integrator.h
	g++ -std=c++11 -c $<

parallel.o: core/parallel.cpp core/parallel.h
	g++ -std=c++11 -c $<

film.o: core/film.cpp core/film.h
	g++ -std=c++11 -c $<

memory.o: core/memory.cpp core/memory.h
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
