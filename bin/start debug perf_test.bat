@for /F "delims=" %%I in ("%~dp0") do @set root=%%~dI

@set PATH=%root%\MinGW\bin;%PATH%

gdb TDef_perf_test -ex run -ex bt -ex q