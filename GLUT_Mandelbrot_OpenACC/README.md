####Compile with:
pgc++ -fast -Minfo -acc main.cpp -lglut -lGL -lm -o MandelbrotACC.bin

####Or if you want to visualize those 2-cycle length sequences then:
pgc++ -Minfo -Mfcon -acc main.cpp -lglut -lGL -lm -o MandelbrotACC.bin -D VIS_REUSE
