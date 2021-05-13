use Time;
// Test single var using the read/write methods

var done: sync bool = true;

proc foo(type t, v: t, s) {
  var d: single t;
  sync {
    begin {
      writeln("2: got ", d.readFF());
      writeln("2: got ", d.readFF());
      done.writeEF(true);
    }
    if done.readFE() then // wait until all prior invocations have finished
      writeln("1: going to sleep with ", v, " of type ", s);
    sleep(1);
    writeln("1: woke up. writing ", v);
    d.writeEF(v);
  }
}

foo(bool, true, "bool");
foo(int(8), 1, "int(8)");
foo(int(16), 2, "int(16)");
foo(int(32), 3, "int(32)");
foo(int(64), 4, "int(64)");
foo(int, 5, "int");
foo(uint, 6, "uint");
foo(real, 7.0, "real");
foo(real(32), 8.0: real(32), "real(32)");
foo(real(64), 9.0, "real(64)");
//foo(complex(64), 10.0: complex(64), "complex(64)");
//foo(complex(128), 11.0: complex(128), "complex(128)");
foo(string, "Hello!", "string");
//type r = range;
//foo(r, 1..3, "range");
