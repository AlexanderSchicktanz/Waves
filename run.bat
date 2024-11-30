@echo off
gcc main.c -o main.exe -lraylib
strip main.exe
main.exe