@echo off
gcc simulation.c -o simulation.exe -lraylib
strip simulation.exe
simulation.exe