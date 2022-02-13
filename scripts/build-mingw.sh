#!/bin/bash

clean=0
debug=0
release=0

while [ -n "$1" ]; do # while loop starts
	case "$1" in
	--clean) echo "--clean option passed, regenerating all build files" 
           clean=1 ;; 
  -D) echo "-D passed, setting BUILD_TYPE to DEBUG"
      debug=1 ;;
  -R) echo "-R passed, setting BUILD_TYPE to RELEASE"
      release=1 ;;
  -h) echo "Usage: "
      echo "  --clean: deletes build directory, rebuilds all"
      echo "       -D: sets BUILD_TYPE to DEBUG"
      echo "       -R: sets BUILD_TYPE to RELEASE"
      exit 1
      ;;
	*) echo "Option $1 not recognized" ;; # In case you typed a different option other than a,b,c
	esac

	shift
done

if [ "$debug" -eq "0" ] && [ "$release" -eq "0" ]
then
  echo "ERROR: SELECT A BUILD_TYPE"
  exit 1
fi
 

if [ "$clean" -eq "1" ]
then
  # if clean is selected, remove the entire build directory
  rm -rf build
else
  # else, just remove the project files
  rm -rf build/exe
  rm -rf build/res
  rm -rf build/*.cc
  rm -rf build/mingw/CMakeFiles/OpenGL_Test.dir
  rm -rf build/mingw/CMakeFiles/Example.dir
fi


# run this from the base project directory
mkdir -p build/mingw
mkdir -p build/exe
cd build/mingw
if [ "$debug" -eq "1" ]
then
  cmake --no-warn-unused-cli -DCMAKE_EXPORT_COMPILE_COMMANDS:BOOL=TRUE -DCMAKE_BUILD_TYPE:STRING=Debug -DCMAKE_C_COMPILER=gcc -DCMAKE_CXX_COMPILER=g++ -G "MinGW Makefiles" ../..
else
  cmake --no-warn-unused-cli -DCMAKE_EXPORT_COMPILE_COMMANDS:BOOL=TRUE -DCMAKE_BUILD_TYPE:STRING=Release -DCMAKE_C_COMPILER=gcc -DCMAKE_CXX_COMPILER=g++ -G "MinGW Makefiles" ../..
fi
make -j 16

cd ../
for file in $(find . -name "*.cc")
do
  #echo $file
  ln -s ".$file" exe/
done