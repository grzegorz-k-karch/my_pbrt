BIN = pbrt

SOURCEDIR = src
# set BUILDDIR only if not set as make parameter
BUILDDIR ?= build

MAIN_OBJECT = pbrt.o
CORE_OBJECTS = quaternion.o material.o primitive.o efloat.o texture.o scene.o integrator.o parallel.o film.o memory.o shape.o sampler.o light.o transform.o error.o interaction.o
CORE_SOURCES = $(CORE_OBJECTS:.o=.cpp)
CORE_DIR = $(SOURCEDIR)/core

ACCELERATOR_OBJECTS = bvh.o
INTEGRATOR_OBJECTS = whitted.o
SHAPE_OBJECTS = triangle.o sphere.o

all: $(BUILDDIR) $(BIN)

$(BUILDDIR):
	mkdir -p $(BUILDDIR)

$(BIN): $(MAIN_OBJECT) $(CORE_OBJECTS) # $(ACCELERATOR_OBJECTS) $(INTEGRATOR_OBJECTS) $(SHAPE_OBJECTS)
	g++ $^ -o $@

$(MAIN_OBJECT): $(SOURCEDIR)/main/$(MAIN_OBJECT:.o=.cpp)
	g++ -std=c++11 -c $< -o $(BUILDDIR)/$(MAIN_OBJECT)

$(CORE_OBJECTS): $(CORE_DIR)/%.o


$(CORE_DIR)/%.o: $(SOURCEDIR)/core/%.cpp
	g++ -std=c++11 -c $< -o $(BUILDDIR)/$@



# $(CORE_OBJECTS)/%.o: $(SOURCEDIR)/core/%.cpp
# 	g++ -std=c++11 -c $<

# $(CORE_OBJECTS): $(SOURCEDIR)/core/$(CORE_OBJECTS:.o=.cpp)
# 	g++ -std=c++11 -c $(SOURCEDIR)/core/$(CORE_OBJECTS:.o=.cpp)

# quaternion.o: core/quaternion.cpp core/quaternion.h
# 	g++ -std=c++11 -c $<

# scene.o: core/scene.cpp core/scene.h
# 	g++ -std=c++11 -c $<

# material.o: core/material.cpp core/material.h
# 	g++ -std=c++11 -c $<

# primitive.o: core/primitive.cpp core/primitive.h
# 	g++ -std=c++11 -c $<

# texture.o: core/texture.cpp core/texture.h
# 	g++ -std=c++11 -c $<

# integrator.o: core/integrator.cpp core/integrator.h
# 	g++ -std=c++11 -c $<

# parallel.o: core/parallel.cpp core/parallel.h
# 	g++ -std=c++11 -c $<

# film.o: core/film.cpp core/film.h
# 	g++ -std=c++11 -c $<

# memory.o: core/memory.cpp core/memory.h
# 	g++ -std=c++11 -c $<

# efloat.o: core/efloat.cpp core/efloat.h
# 	g++ -std=c++11 -c $<

# shape.o: core/shape.cpp core/shape.h
# 	g++ -std=c++11 -c $<

# sampler.o: core/sampler.cpp core/sampler.h
# 	g++ -std=c++11 -c $<

# light.o: core/light.cpp core/light.h
# 	g++ -std=c++11 -c $<

# transform.o: core/transform.cpp core/transform.h
# 	g++ -std=c++11 -c $<

# error.o: core/error.cpp core/error.h
# 	g++ -std=c++11 -c $<

# interaction.o: core/interaction.cpp core/interaction.h
# 	g++ -std=c++11 -c $<

# ##
# bvh.o: accelerators/bvh.cpp accelerators/bvh.h
# 	g++ -std=c++11 -c $< -Icore

# ##
# sphere.o: shapes/sphere.cpp shapes/sphere.h
# 	g++ -std=c++11 -c $< -Icore

# triangle.o: shapes/triangle.cpp shapes/triangle.h
# 	g++ -std=c++11 -c $< -Icore

# ##
# whitted.o: integrators/whitted.cpp integrators/whitted.h
# 	g++ -std=c++11 -c $< -Icore

## Clean object files, temporary files, executable
clean:
	rm -f ./*~ ./*.o 
