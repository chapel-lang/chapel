// This test uses the --force-initializers flag to generate a default
// initializer instead of a default constructor.  It is expected that in the
// future, default initializers will automatically be generated instead of
// default constructors and that this flag will first be only useful for
// verifying behavior against default constructors, and then be removed once
// support for constructors is removed.  The exact timeframe on those steps is
// unclear.
class LotsOFields {
  var f1: int;
  var f2: real = 14.3;
  const f3 = false;
}

proc main() {
  var c: LotsOFields = new LotsOFields(2, 6.3, true);

  writeln(c);
  delete c;
}
