#!/bin/bash

clean=0

while [ -n "$1" ]; do # while loop starts
	case "$1" in
	--clean) echo "--clean option passed, regenerating all build files" 
           clean=1 ;; 
	*) echo "Option $1 not recognized" ;; # In case you typed a different option other than a,b,c
	esac

	shift
done
 

if [ "$clean" -eq "1" ]
then
  echo "removing build directory"
  # if clean is selected, remove the entire build directory
  rm -rf build
else
  # else, just remove the project files
  rm -rf build/exe
  rm -rf build/res
  rm -rf build/*.cc
  rm -rf build/clang/CMakeFiles/OpenGL_Test.dir
  rm -rf build/clang/CMakeFiles/Example.dir
fi

mkdir -p build/clang
mkdir -p build/exe
cd build/clang
cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_C_COMPILER=clang -DCMAKE_CXX_COMPILER=clang++ ../..
make -j 16

# go to build/, make symlinks to executables in build/exe/*.cc
cd ../
for file in $(find . -name "*.cc")
do
    ln -s ".$file" exe/
done