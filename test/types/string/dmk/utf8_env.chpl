//
// This test is specific to the testing environment, which
// sets the character-set environment variables in a
// predetermined way.  The test may yield different results
// when run outside the test harness.
//
extern proc getenv(const name: c_string): c_string;
writeln("LANG=", getenv('LANG'):string);
writeln("LC_ALL=", getenv('LC_ALL'):string);
writeln("LC_COLLATE=", getenv('LC_COLLATE'):string);
