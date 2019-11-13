//
// This test is specific to the testing environment, which
// sets the character-set environment variables in a
// predetermined way.  The test may yield different results
// when run outside the test harness.
//
extern proc getenv(const name: c_string): c_string;
writeln("LANG=", string.createWithNewBuffer(getenv('LANG')));
writeln("LC_ALL=", string.createWithNewBuffer(getenv('LC_ALL')));
writeln("LC_COLLATE=", string.createWithNewBuffer(getenv('LC_COLLATE')));
