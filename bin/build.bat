@for /F "delims=" %%I in ("%~dp0") do @set root=%%~dI

@set PATH=%root%\devel\MinGW\bin;%PATH%


mingw32-gcc ../src/*.c ../src/*.cpp -lSDL2main -lSDL2 -lSDL2_net -lSDL2_mixer -lopengl32 -lglu32 -lstdc++ -Wall -fgnu89-inline -g -o TDef
pause