#Promotion Tests Readme

The script in this directory is used to create the .chpl files in the parent directory.
Only monadic (single-argument) math functions are currently handled.  There are a few
dyadic functions that need do not fit into this framework.

Each line in "monadic" contains the name of the math function and a domain over which the
function is to be exercised.  The domain is traversed in 10 uniform steps (11 samples).

The same driver is created for each math function, and the name of the math function and
input domain substituted.  A different driver and perhaps independent input domains.

The .good files were created by running the tests and then copying over the resulting
<test>.exec.out.tmp to <test>.good.

