@echo off
cd .\build\
cmake --build .
cd .\Debug\
.\AirplanesLiveClone.exe
cd ../..