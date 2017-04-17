remake: clean all

all: medMelt

medMelt: medMelt.cpp erickO.cpp cesarN.cpp robertP.cpp zackO.cpp
	 g++ medMelt.cpp erickO.cpp cesarN.cpp robertP.cpp zackO.cpp joystick.c log.cpp ppm.cpp libggfonts.a /usr/lib/x86_64-linux-gnu/libopenal.so /usr/lib/x86_64-linux-gnu/libalut.so -Wall -omedMelt -lX11 -lGL -lGLU -lm

clean: 
	rm -f medMelt
	rm -f *.o
