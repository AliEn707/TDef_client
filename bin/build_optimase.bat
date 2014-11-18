@for /F "delims=" %%I in ("%~dp0") do @set root=%%~dI

@set PATH=%root%\MinGW\bin;%PATH%


mingw32-gcc *.c ../*.c ../*.cpp -lSDL2main -lSDL2 -lSDL2_net -lopengl32 -lglu32 -lstdc++ -O3 -fgnu89-inline -ffast-math -flto -fsingle-precision-constant -Wall -Wextra  -g -o TDef 
pause