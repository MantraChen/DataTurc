@echo off
g++ -std=gnu++20 -O2 -Wall -I include "%1" -o "%~dpn1.exe"
if errorlevel 1 exit /b
"%~dpn1.exe"
