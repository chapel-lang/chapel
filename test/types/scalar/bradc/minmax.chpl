
proc printRange(type t, name) {
  var minval: t = min(t);
  var maxval: t = max(t);
  writeln("Type ", name, " ranges from ", minval, "..", maxval);
}

printRange(int, "int");
printRange(int(8), "int(8)");
printRange(int(16), "int(16)");
printRange(int(32), "int(32)");
printRange(int(64), "int(64)");

printRange(uint, "uint");
printRange(uint(8), "uint(8)");
printRange(uint(16), "uint(16)");
printRange(uint(32), "uint(32)");
printRange(uint(64), "uint(64)");

printRange(real, "real");
printRange(real(32), "real(32)");
printRange(real(64), "real(64)");

printRange(complex, "complex");
printRange(complex(64), "complex(64)");
printRange(complex(128), "complex(128)");
