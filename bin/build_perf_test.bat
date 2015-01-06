@for /F "delims=" %%I in ("%~dp0") do @set root=%%~dI
@for /F "delims=" %%I in ("%~dp0") do @set build_root=%%~fI

@set PATH=%root%\devel\MinGW\bin;%PATH%

@cd /d %temp%
mingw32-gcc  %build_root%..\src\*.c %build_root%..\src\perf_test\*.c -lSDL2main -lSDL2 -lSDL2_net -lopengl32 -lglu32 -lstdc++ -Wall -fgnu89-inline -g -c
mingw32-gcc *.o  %build_root%..\src\*.cpp -lSDL2main -lSDL2 -lSDL2_net -lopengl32 -lglu32 -lstdc++ -Wall -g -o %build_root%TDef_perf_test
@rm -rf *.o
pause