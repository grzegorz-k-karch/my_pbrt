all: pbrt

OBJS=point.o api.o constant.o matte.o plastic.o \
reflection.o bvh.o boxFilter.o triangleFilter.o \
gaussianFilter.o sincFilter.o filter.o stratified.o \
camera.o spectrum.o quaternion.o material.o \
primitive.o pbrt.o sphere.o efloat.o triangle.o \
texture.o scene.o integrator.o parallel.o film.o \
memory.o shape.o sampler.o sampling.o whitted.o \
directlighting.o light.o transform.o error.o \
interaction.o

pbrt: ${OBJS} 
	g++ $^ -o $@

pbrt.o: main/pbrt.cpp
	g++ -std=c++11 -Icore -c $^

reflection.o: core/reflection.cpp core/reflection.h
	g++ -std=c++11 -c $<

api.o: core/api.cpp core/api.h
	g++ -std=c++11 -c $<

quaternion.o: core/quaternion.cpp core/quaternion.h
	g++ -std=c++11 -c $<

boxFilter.o: filters/box.cpp filters/box.h
	g++ -std=c++11 -c $< -Icore -o boxFilter.o

point.o: lights/point.cpp lights/point.h
	g++ -std=c++11 -c $< -Icore -o point.o

triangleFilter.o: filters/triangle.cpp filters/triangle.h
	g++ -std=c++11 -c $< -Icore -o triangleFilter.o

gaussianFilter.o: filters/gaussian.cpp filters/gaussian.h
	g++ -std=c++11 -c $< -Icore -o gaussianFilter.o

sincFilter.o: filters/sinc.cpp filters/sinc.h
	g++ -std=c++11 -c $< -Icore -o sincFilter.o

filter.o: core/filter.cpp core/filter.h
	g++ -std=c++11 -c $<

stratified.o: samplers/stratified.cpp samplers/stratified.h
	g++ -std=c++11 -c $< -Icore

matte.o: materials/matte.cpp materials/matte.h
	g++ -std=c++11 -c $< -Icore

plastic.o: materials/plastic.cpp materials/plastic.h
	g++ -std=c++11 -c $< -Icore

constant.o: textures/constant.cpp textures/constant.h
	g++ -std=c++11 -c $< -Icore

camera.o: core/camera.cpp core/camera.h
	g++ -std=c++11 -c $<

orthographic.o: cameras/orthographic.cpp cameras/orthographic.h
	g++ -std=c++11 -c $< -Icore

perspective.o: cameras/perspective.cpp cameras/perspective.h
	g++ -std=c++11 -c $< -Icore

spectrum.o: core/spectrum.cpp core/spectrum.h
	g++ -std=c++11 -c $<

sphere.o: shapes/sphere.cpp shapes/sphere.h
	g++ -std=c++11 -c $< -Icore

bvh.o: accelerators/bvh.cpp accelerators/bvh.h
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

sampling.o: core/sampling.cpp core/sampling.h
	g++ -std=c++11 -c $<

light.o: core/light.cpp core/light.h
	g++ -std=c++11 -c $<

transform.o: core/transform.cpp core/transform.h
	g++ -std=c++11 -c $<

error.o: core/error.cpp core/error.h
	g++ -std=c++11 -Iext/glog/build -Iext/glog/src -c $<

interaction.o: core/interaction.cpp core/interaction.h
	g++ -std=c++11 -c $<

whitted.o: integrators/whitted.cpp integrators/whitted.h
	g++ -std=c++11 -c $< -Icore

directlighting.o: integrators/directlighting.cpp integrators/directlighting.h
	g++ -std=c++11 -c $< -Icore

clean:
	rm -f ./*~ ./*.o pbrt
