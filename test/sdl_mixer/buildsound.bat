@for /F "delims=" %%I in ("%~dp0") do @set root=%%~dI

@set PATH=%root%\devel\MinGW\bin;%PATH%

mingw32-gcc SDL_PlaySound.c -lSDL2main -lSDL2 -lSDL2_mixer  -lwinmm -o ogg

pause