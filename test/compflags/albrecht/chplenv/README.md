This test ensures that the compiler flags associated with the CHPL_variables
printed in printchplenv properly override the environment variables.

This is done by setting the CHPL_variables to "bad" and passing the correct
values through compiler flags
