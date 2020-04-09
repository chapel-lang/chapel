config param nonzero = 42,
             zero = 0;

proc testType(param size) {
  param ti: int(size) = nonzero: int(size),
        tu: uint(size) = nonzero: uint(size),
        fi: int(size) = zero: int(size),
        fu: uint(size) = 0: uint(size);

  if (!ti) {
    compilerWarning("ti is zero");
    writeln("ti is zero");
  } else {
    writeln("ti is nonzero");
    compilerWarning("ti is nonzero");
  }

  if (!tu) {
    writeln("tu is zero");
    compilerWarning("tu is zero");
  } else {
    writeln("tu is nonzero");
    compilerWarning("tu is nonzero");
  }

  if (!fi) {
    writeln("fi is zero");
    compilerWarning("fi is zero");
  } else {
    writeln("fi is nonzero");
    compilerWarning("fi is nonzero");
  }

  if (!fu) {
    writeln("fu is zero");
    compilerWarning("fu is zero");
  } else {
    writeln("fu is nonzero");
    compilerWarning("fu is nonzero");
  }
}

testType(8);
testType(16);
testType(32);
testType(64);

