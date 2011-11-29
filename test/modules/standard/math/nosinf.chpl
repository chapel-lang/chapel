var x: real(32);

// This is intended to check that we don't accidentally leak external
// C routines into the Chapel user's namespace

writeln("sinf(x) = ", sinf(x));
