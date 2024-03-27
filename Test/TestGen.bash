#!/bin/bash

# generation rvdashTests
rm ../Test/rvdashTests/GenTests.h
NumTest=1
for i in $( ls ../Test/rvdashTests/Data ); do
  echo "ADD_TEST("$NumTest", rvdash)" >> GenTests.h
  let NumTest=NumTest+1
done
mv GenTests.h ../Test/rvdashTests
mkdir ../Test/rvdashTests/Results
echo "All tests for rvdash are generated!"


# generation ErrorHandingTests
rm ../Test/ErrorHandlingTests/GenTests.h
NumTest=1
for i in $( ls ../Test/ErrorHandlingTests/Data ); do
  echo "ADD_TEST("$NumTest", ErrorHandling)" >> GenTests.h
  let NumTest=NumTest+1
done
mv GenTests.h ../Test/ErrorHandlingTests
mkdir ../Test/ErrorHandlingTests/Results
echo "All tests for rvdash Error Handling are generated!"






