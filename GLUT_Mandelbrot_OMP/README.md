####Compile with:
g++ main.cpp -lm -lglut -lGL -openmp -fopenmp -Ofast -o MandelOMP.bin

####Or if you want to visualize those 2-cycle length sequences then:
g++ main.cpp -lm -lglut -lGL -openmp -fopenmp -Ofast -o MandelOMP.bin -D VIS_REUSE
