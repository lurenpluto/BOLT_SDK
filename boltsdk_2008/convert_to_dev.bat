
REM mkdir .\dev\pr
REM mkdir .\dev\pr\bin
REM mkdir .\dev\pr\lib

REM :备份pr版
REM copy /y .\bin\*.dll .\dev\pr\bin 
REM copy /y .\lib\*.lib .\dev\pr\lib 

:切换为开发版
copy /y .\dev\dev\bin\*.dll .\bin
copy /y .\dev\dev\lib\*.lib .\lib
copy /y .\dev\dev\bin\*.exe .\bin

