@for /F "delims=" %%I in ("%~dp0") do @set root=%%~dI

@set PATH=%root%\MinGW\bin;%PATH%


mingw32-gcc ../src/*.c ../src/*.cpp -lSDL2main -lSDL2 -lSDL2_net -lopengl32 -lglu32 -lstdc++ -O3 -fgnu89-inline -ffast-math -flto -fsingle-precision-constant -ffloat-store -mno-align-double -mfpmath=sse -mmmx -msse4 -Wall -Wextra  -g -o TDef 
pause