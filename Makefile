CC=clang++
CFLAGS=-Wall -I/usr/X11R6/include
LDFLAGS= -L/usr/X11R6/lib -lm  -lglfw3 -lglew -framework Cocoa -framework OpenGL -framework IOKit -framework CoreVideo
OBJS=main.o shader.o
TST=test.o
#scene.o shader.o

all: clean glsl_lighting

glsl_lighting:	$(OBJS)
	$(CC) $(LDFLAGS) $(OBJS) -o glsl_lighting
test:	$(TST)
	$(CC) $(LDFLAGS) $(TST) -o test	
lib:	
	$(CC) -dynamiclib -mmacosx-version-min=10.5 $(LDFLAGS) -o libShaders.dylib -dy main.cpp
clean:
	rm -f glsl_lighting
	rm -f $(OBJS)
	rm -f $(TST)

main.o: main.cpp
scene.o: scene.c
shader.o: shader.c
test.o: test.cpp