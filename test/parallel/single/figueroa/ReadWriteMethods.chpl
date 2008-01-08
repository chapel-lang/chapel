use Time;
// Test single field using the read/write methods

var done: sync bool = true;

def foo(type t, v: t, s) {
  var d: single t;

  begin {
    writeln("2: got ", d.readFF());
    writeln("2: got ", d.readFF());
    done = true;
  }
  if done then // wait until all prior invocations have finished
  writeln("1: going to sleep with ", v, " of type ", s);
  sleep(1);
  writeln("1: woke up. writing ", v);
  d.writeEF(v);
}

foo(int(8), 4, "int(8)");
foo(int(16), 4, "int(16)");
foo(int(32), 4, "int(32)");
foo(int(64), 4, "int(64)");
foo(int, 4, "int");
foo(uint, 4, "uint");
foo(real, 4.0, "real");
foo(real(32), 4.0: real(32), "real(32)");
foo(real(64), 4.0, "real(64)");
foo(complex(64), 4.0: complex(64), "complex(64)");
foo(complex(128), 4.0: complex(128), "complex(128)");
foo(string, "4.", "string");
type r = range;
foo(r, 1..3, "range");
