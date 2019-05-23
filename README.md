# Project laboratory

This will be a snow simulation application, that display snow based on moving particles.
Currently, it loads a the prt files from a simulation directory, and generates additional moving  particles.  

## Compile with visual studio
open git bash:
```
git clone --recursive https://github.com/pr1stvan/project_laboratory.git
cd project_laboratory
mkdir build
cd build
cmake .. -G "Visual Studio 15 2017"
```
Currently, it only works in release mode.

## Compile with mingw

open git bash:
```
git clone --recursive https://github.com/pr1stvan/project_laboratory.git
cd project_laboratory
mkdir build
cd build
cmake .. -G "MinGW Makefiles" -DCMAKE_BUILD_TYPE=Release -DCMAKE_SH="CMAKE_SH-NOTFOUND"
mingw32-make
./project_laboratory
```
## Compile on ubuntu
open terminal:
```
git clone --recursive https://github.com/pr1stvan/project_laboratory.git
cd project_laboratory
mkdir build
cd build
cmake .. --DCMAKE_BUILD_TYPE=Release
make
./project_laboratory
```
