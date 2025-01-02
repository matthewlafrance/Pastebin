#!/bin/sh

# paste test file
# list pastes and save
# compare to original IDs

make
target/main paste test < tests/inputs/test.txt
target/main ls > /tmp/test_output.txt
echo "test" > /tmp/expected_output.txt
diff /tmp/test_output.txt /tmp/expected_output.txt
target/main rm test
