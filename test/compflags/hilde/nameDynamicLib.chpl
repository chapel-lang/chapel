// nameDynamicLib.chpl
//
// Test to see if we can name a dynamic library on the command line.
//
// This is just dummy code that needs stuff in the GMP library.
// The interesting part of this test is in nameDynamicLib.compopts.
//

use GMP;

var tmp1: mpz_t;

mpz_init(tmp1);

