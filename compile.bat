@echo off
gcc SimShader.c -o SimShader.exe -lraylib
strip SimShader.exe
SimShader.exe