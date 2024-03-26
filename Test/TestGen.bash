#!/bin/bash
rm GenTests.h
COUNTER=1
for i in $( ls ../Test/rvdashTests/Data ); do
  echo "ADD_TEST("$COUNTER")" >> GenTests.h
  let COUNTER=COUNTER+1
done
mv GenTests.h ../Test
echo "All tests for rvdash are generated!"
