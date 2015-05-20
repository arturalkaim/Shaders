CC=clang++
CFLAGS=-Wall -I/usr/X11R6/include
LDFLAGS= -L/usr/X11R6/lib -lm  -lglfw3 -lglew -framework Cocoa -framework OpenGL -framework IOKit -framework CoreVideo
OBJS=main.o
TST=test.o
OGL=plainOpengl.o
#scene.o shader.o

all: clean myGDTool

myGDTool:	$(OBJS)
	$(CC) $(LDFLAGS) $(OBJS) -o myGDTool
test:	$(TST)
	$(CC) $(LDFLAGS) $(TST) -o test	
ogl: 	$(OGL)
	$(CC) $(LDFLAGS) $(OGL) -o plainOGL
lib:	
	$(CC) -dynamiclib -mmacosx-version-min=10.5 $(LDFLAGS) -o libShaders.dylib -dy main.cpp
clean:
	rm -f myGDTool
	rm -f $(OBJS)
	rm -f $(TST)

main.o: main.cpp
test.o: test.cpp
plainOpengl.o: plainOpengl.cpp