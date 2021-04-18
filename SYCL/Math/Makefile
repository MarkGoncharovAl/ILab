CC=clang++
SYCL=-fsycl
DEF_FLAGS=-O3

MySycl=build/MySycl.o
Geometry=build/geometry.o

all: MySycl Geometry
	$(CC) $(SYCL) $(DEF_FLAGS) main.cpp $(MySycl) $(Geometry) -lLogging -o main
	@echo "Files were compiled"

MySycl: ../MySycl/MySycl.cpp
	$(CC) $(SYCL) $(DEF_FLAGS) -c ../MySycl/MySycl.cpp -o $(MySycl)
Geometry: geometry/geometry.cpp
	$(CC) $(SYCL) $(DEF_FLAGS) -c geometry/geometry.cpp -o $(Geometry)

clean:
	@rm -r build/*
	@echo "build was cleaned"