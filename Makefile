remake: clean all

all: medMelt medMelt-Sound

medMelt: medMelt.cpp erickO.cpp cesarN.cpp robertP.cpp zackO.cpp
	 g++ medMelt.cpp erickO.cpp cesarN.cpp robertP.cpp zackO.cpp joystick.c log.cpp ppm.cpp libggfonts.a -Wall -omedMelt -lX11 -lGL -lGLU -lm

medMelt-Sound: medMelt.cpp erickO.cpp cesarN.cpp robertP.cpp zackO.cpp
	 g++ medMelt.cpp erickO.cpp cesarN.cpp robertP.cpp zackO.cpp joystick.c log.cpp ppm.cpp libggfonts.a /usr/lib/x86_64-linux-gnu/libopenal.so /usr/lib/x86_64-linux-gnu/libalut.so -Wall -omedMelt-Sound -D USE_OPENAL_SOUND -lX11 -lGL -lGLU -lm


clean: 
	rm -f medMelt
	rm -f medMelt-Sound
	rm -f *.o
