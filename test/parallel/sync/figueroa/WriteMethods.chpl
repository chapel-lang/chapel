use Time;
// Test sync var using mostly write methods

proc foo(type t, u: t, v: t, name) {
  var s: sync t;

  writeln("going to sleep ... ");
  begin {
    s.writeFF(u);
    s.writeEF(v);
  }
  sleep(1);
  writeln("woke up. initial value is ", s.readXX(), " of type ", name);
  s.writeXF(v);
  sleep(1);
  writeln("value is now ", s.readFE());
  writeln("value has changed to ", s.readFF());
  s.writeXF(u);
  write  ("The final value is ", s.readFE());
  s.reset();
  writeln(", but it was reset to ", s.readXX());
}

foo(bool, true, false, "bool");
foo(int(8), 1, 5, "int(8)");
foo(int(16), 2, 6, "int(16)");
foo(int(32), 3, 7, "int(32)");
foo(int(64), 4, 8, "int(64)");
foo(int, 5, 9, "int");
foo(uint, 6, 10, "uint");
foo(real, 7.0, 11.0, "real");
foo(real(32), 8.0: real(32), 12.0: real(32), "real(32)");
foo(real(64), 9.0, 13.0, "real(64)");
foo(complex(64), 10.0: complex(64), 14.0: complex(64), "complex(64)");
foo(complex(128), 11.0: complex(128), 15.0: complex(128), "complex(128)");
foo(imag, 12.0i, 16.0i, "imag");
foo(string, "Hello,", "world!", "string");
type r = range;
foo(r, 1..3, 4..7, "range");
