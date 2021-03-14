// Verify that an error is reported when assigning to a would-be local.

#include "support-decls.cpp"

proc test() {
#include "var-decls.cpp"
#include "error-base.cpp"
} //test()

test();
