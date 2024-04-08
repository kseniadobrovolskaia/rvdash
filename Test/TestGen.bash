#!/bin/bash

# This script runs from the CMakeLists.txt. 
# Its first argument $1 is the path to the Test directory. 
# It generates in each test subdirectory (rvdashTests, ErrorHandlingTests)
# a header file GenTests.h with ADD_TEST macros. 
# This file is included in the test file Test.cpp and Google test
# functions TEST are created for each file from the Data directory.


# generation rvdashTests
if [ -f "$1/rvdashTests/GenTests.h" ]; then
  rm $1/rvdashTests/GenTests.h
fi
NumTest=1
for i in $( ls $1/rvdashTests/Data ); do
  echo "ADD_TEST("$NumTest", rvdash)" >> GenTests.h
  let NumTest=NumTest+1
done
mv GenTests.h $1/rvdashTests
if [ ! -d "$1/rvdashTests/Results" ]; then
  mkdir $1/rvdashTests/Results
fi
echo "All tests for rvdash are generated!"


# generation ErrorHandingTests
if [ -f "$1/ErrorHandlingTests/GenTests.h" ]; then
  rm $1/ErrorHandlingTests/GenTests.h
fi
NumTest=1
for i in $( ls $1/ErrorHandlingTests/Data ); do
  echo "ADD_TEST("$NumTest", ErrorHandling)" >> GenTests.h
  let NumTest=NumTest+1
done
mv GenTests.h $1/ErrorHandlingTests
if [ ! -d "$1/ErrorHandlingTests/Results" ]; then
  mkdir $1/ErrorHandlingTests/Results
fi
echo "All tests for rvdash Error Handling are generated!"


# generation SnippyRVdashTests
if [ ! -d "$1/Snippy" ]; then
  echo "Downloading Snippy from GitHub"
  mkdir $1/Snippy
  cd $1/Snippy
  wget https://github.com/syntacore/snippy/releases/download/snippy-1.0/snippy-1.0.tar.gz
  tar -xvzf snippy-1.0.tar.gz
  cd $1
fi
if [ -f "$1/SnippyRVdashTests/GenTests.h" ]; then
  rm $1/SnippyRVdashTests/GenTests.h
fi
NumTest=1
for i in $( ls $1/SnippyRVdashTests/Data ); do
  echo "ADD_SNIPPY_MODEL_TEST("$NumTest", SnippyRVdash)" >> GenTests.h
  let NumTest=NumTest+1
done
mv GenTests.h $1/SnippyRVdashTests
if [ ! -d "$1/SnippyRVdashTests/Results" ]; then
  mkdir $1/SnippyRVdashTests/Results
fi
echo "All tests for SnippyRVdash are generated!"


