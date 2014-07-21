mingw32-gcc client.c -lSDL2main -lSDL2 -lopengl32 -lglu32 -lwinmm -lws2_32 -lmswsock -o client -g 
mingw32-gcc server.c -lSDL2main -lSDL2 -lopengl32 -lglu32 -lwinmm -lws2_32 -lmswsock -o server -g 
pause