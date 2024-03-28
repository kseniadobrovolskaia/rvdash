#!/bin/bash

# generation rvdashTests
rm $1/rvdashTests/GenTests.h
NumTest=1
for i in $( ls $1/rvdashTests/Data ); do
  echo "ADD_TEST("$NumTest", rvdash)" >> GenTests.h
  let NumTest=NumTest+1
done
mv GenTests.h $1/rvdashTests
mkdir $1/rvdashTests/Results
echo "All tests for rvdash are generated!"


# generation ErrorHandingTests
rm $1/ErrorHandlingTests/GenTests.h
NumTest=1
for i in $( ls $1/ErrorHandlingTests/Data ); do
  echo "ADD_TEST("$NumTest", ErrorHandling)" >> GenTests.h
  let NumTest=NumTest+1
done
mv GenTests.h $1/ErrorHandlingTests
mkdir $1/ErrorHandlingTests/Results
echo "All tests for rvdash Error Handling are generated!"






