// Test sync var using mostly read, reset, and isFull methods

var done: sync bool = true;

proc foo(type t, u: t, v: t, name) {
  var s: sync t;

  if done.readFE() then // wait until all prior invocations have finished
  writeln("1: going to sleep ... ");

  sync {
    begin {
      writeln("2: initial value is ", s.readXX(), " of type ", name);
      done.writeEF(true);
      write  ("2: value is now ", s.readFE());
      writeln(" and it is ", if s.isFull then "full" else "empty");
      done.writeEF(true);
      write  ("2: value has changed to ", s.readFF());
      writeln(" and it is ", if s.isFull then "full" else "empty");
      chpl_task_yield();
      writeln("2: after sleeping, value is still ", s.readXX());
      s.reset();
      writeln("2: value has been reset to ", s.readFE());
      done.writeEF(true);
    }
    done.readFE();
    writeln("1: woke up. writing ", u);
    s.writeEF(u);
    if done.readFE() then s.writeEF(v);
    s.writeEF(u);
  }
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
//foo(complex(64), 10.0: complex(64), 14.0: complex(64), "complex(64)");
//foo(complex(128), 11.0: complex(128), 15.0: complex(128), "complex(128)");
foo(imag, 12.0i, 16.0i, "imag");
foo(string, "Hello,", "world!", "string");
//type r = range;
//foo(r, 1..3, 4..7, "range");
