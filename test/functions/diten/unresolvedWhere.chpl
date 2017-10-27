proc foo(x) where isIntegral(x) { }
proc foo(x) where isReal(x) { }
foo("bar");
