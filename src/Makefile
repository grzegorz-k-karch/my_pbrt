all: pbrt

pbrt: pbrt.o scene.o integrator.o parallel.o film.o memory.o sampler.o
	g++ $^ -o $@

pbrt.o: main/pbrt.cpp
	g++ -std=c++11 -c $^

scene.o: core/scene.cpp core/scene.h
	g++ -std=c++11 -c core/scene.cpp

integrator.o: core/integrator.cpp core/integrator.h
	g++ -std=c++11 -c core/integrator.cpp

parallel.o: core/parallel.cpp core/parallel.h
	g++ -std=c++11 -c core/parallel.cpp

film.o: core/film.cpp core/film.h
	g++ -std=c++11 -c core/film.cpp

memory.o: core/memory.cpp core/memory.h
	g++ -std=c++11 -c core/memory.cpp

sampler.o: core/sampler.cpp core/sampler.h
	g++ -std=c++11 -c core/sampler.cpp

clean:
	rm -f ./*~ ./*.o pbrt
