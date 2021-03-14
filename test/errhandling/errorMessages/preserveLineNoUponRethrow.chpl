
proc willthrow() throws {
  writeln("in willthrow");
  throw new owned Error();
}

proc rethrow() throws {
  try {
    writeln("in rethrow");
    willthrow();
    writeln("in rethrow after willthrow");
  } catch exceptn {
    writeln("catch in rethrow");
    throw exceptn;
  }
}

proc main throws {
  rethrow();
  writeln("after rethrow");
}
