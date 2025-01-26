#!/bin/bash

# Test cases for arger program

echo "Testing arger with different options:"

# Test uppercase conversion
./arger -u "hello world"
if [ $? -eq 0 ]; then
    echo "Uppercase test passed"
else
    echo "Uppercase test failed"
fi

# Test lowercase conversion
./arger -l "HELLO WORLD"
if [ $? -eq 0 ]; then
    echo "Lowercase test passed"
else
    echo "Lowercase test failed"
fi

# Test capitalization
./arger -cap "hello world"
if [ $? -eq 0 ]; then
    echo "Capitalization test passed"
else
    echo "Capitalization test failed"
fi

# Test invalid option
./arger -invalid "test"
if [ $? -eq -1 ]; then
    echo "Invalid option test passed"
else
    echo "Invalid option test failed"
fi

# Test insufficient arguments
./arger
if [ $? -eq -1 ]; then
    echo "Insufficient arguments test passed"
else
    echo "Insufficient arguments test failed"
fi 