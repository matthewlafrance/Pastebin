#!/bin/sh

# save contents of test file as paste
# view paste and save output
# compare output to test file

make
target/main paste test < tests/inputs/test.txt
target/main view test > /tmp/test_output.txt
diff /tmp/test_output.txt tests/inputs/test.txt
target/main rm test

# extensions:
# - write more test files, generalize this script to all input files in inputs/
# - make the test output look nicer: less unnecessary output from the individual commands, just print test name and result
# - since you're testing rm you may as well also test ls
