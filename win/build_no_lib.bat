@rem build exe with libstdc++ and libgcc

@for /F "delims=" %%I in ("%~dp0") do @set root=%%~dI

@set PATH=%root%\MinGW\bin;%PATH%


mingw32-gcc *.c ../*.c  -lSDL2main -lSDL2 -lSDL2_net -lopengl32 -lglu32 -lwinmm -lws2_32 -static-libgcc -Wall -g  -c 
mingw32-g++ ../*.cpp *.o -lSDL2main -lSDL2 -lSDL2_net -lopengl32 -lglu32 -lwinmm -lws2_32 -static-libstdc++ -static-libgcc -Wall -g -o TDef
del *.o
pause