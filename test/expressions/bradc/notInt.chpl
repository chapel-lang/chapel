config const nonzero = 42,
             zero = 0;

proc testType(param size) {
  var ti: int(size) = nonzero: int(size),
      tu: uint(size) = nonzero: uint(size),
      fi: int(size) = zero: int(size),
      fu: uint(size) = 0: uint(size);

  if (!ti) {
    writeln("ti is zero");
  } else {
    writeln("ti is nonzero");
  }

  if (!tu) {
    writeln("tu is zero");
  } else {
    writeln("tu is nonzero");
  }

  if (!fi) {
    writeln("fi is zero");
  } else {
    writeln("fi is nonzero");
  }

  if (!fu) {
    writeln("fu is zero");
  } else {
    writeln("fu is nonzero");
  }
}

testType(8);
testType(16);
testType(32);
testType(64);

