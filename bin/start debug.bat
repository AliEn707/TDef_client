@for /F "delims=" %%I in ("%~dp0") do @set root=%%~dI

@set PATH=%root%\MinGW\bin;%PATH%

gdb %1 -ex run -ex bt -ex q