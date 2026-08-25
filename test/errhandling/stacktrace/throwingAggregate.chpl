config param doCatch = false;

proc throwIfEq(val, eq) throws {
  if val == eq then
    throw new Error("value is equal to " + eq:string);
}
record R {
  var x: int;
  proc init(val: int) throws {
    x = val;
    init this;
    throwIfEq(val, 0);
  }
  proc postinit() throws {
    throwIfEq(x, 1);
  }
  proc method() throws {
    throwIfEq(x, 2);
  }
}
class C {
  var x: int;
  proc init(val: int) throws {
    x = val;
    init this;
    throwIfEq(val, 0);
  }
  proc postinit() throws {
    throwIfEq(x, 1);
  }
  proc method() throws {
    throwIfEq(x, 2);
  }
}
union U {
  var x: int;
  proc init(val: int) throws {
    x = val;
    init this;
    throwIfEq(val, 0);
  }
  proc postinit() throws {
    throwIfEq(x, 1);
  }
  proc method() throws {
    throwIfEq(x, 2);
  }
}


proc test(type t) {
  for val in 0..2 {
    try {
      writeln("creating ", t:string, " with value ", val);
      var r = new t(val);
      writeln("created ", r);
      r.method();
    } catch e {
      writeln("caught error: ", e.message());
      writeln("stack trace:");
      for ((f, l), idx) in zip(e.stacktrace(), 1..) {
        writeln("  (frame ", idx, ") ", f, ":", l);
      }
    }
  }
}

test(R);
test(owned C);
test(shared C);
test(U);
