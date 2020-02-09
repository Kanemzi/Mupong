@echo off
rem Do not edit! This batch file is created by CASIO fx-9860G SDK.


if exist MAIN.G1A  del MAIN.G1A

cd debug
if exist FXADDINror.bin  del FXADDINror.bin
"C:\Users\CODE\Documents\CASIO_SDK\fx-9860G SDK\OS\SH\Bin\Hmake.exe" Addin.mak
cd ..
if not exist debug\FXADDINror.bin  goto error

"C:\Users\CODE\Documents\CASIO_SDK\fx-9860G SDK\Tools\MakeAddinHeader363.exe" "E:\CASIO_PROGRAMS\MUPONG"
if not exist MAIN.G1A  goto error
echo Build has completed.
goto end

:error
echo Build was not successful.

:end

