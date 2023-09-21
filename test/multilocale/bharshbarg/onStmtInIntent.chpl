
use CTypes;
use Types;
use Communication;
use List;

// TODO: once in-intents work properly, we shouldn't need this anymore.
var pointers : list((locale, c_ptr(int)));
proc alloc() {
  var ret : c_ptr(int) = allocate(int, 1);
  pointers.pushBack((here, ret));
  return ret;
}

record R : writeSerializable {
  var x : c_ptr(int);
  var home : locale;

  proc init() {
    init this;
    this.x = alloc();
    home = here;
    set(42);
  }

  proc init=(other: R) {
    init this;
    this.x = alloc();
    home = here;
    set(other.get());
    writeln("init=");
  }

  proc get() {
    var ret : int;
    var dest = c_ptrTo(ret);
    Communication.get(dest, x, home.id, numBytes(int));
    return ret;
  }

  proc set(in val: int) {
    var src = c_ptrTo(val);
    Communication.put(x, src, home.id, numBytes(int));
  }

  proc serialize(writer, ref serializer) {
    writer.write("{", get(), "}");
  }
}

proc inHelper(in myArg) {
  myArg.set(999);
}

proc foo(arg) {
  on Locales.last {
    inHelper(arg);
  }
}

//
// This program was created to track a bug in the compiler where in-intent
// copies were not inserted when the call was inside an on-statement, but the
// actual was declared somewhere outside the on-statement. This allows for
// the original actual to be modified, which is illegal.
//
// The program below is expected to print 'x' as unchanged before and after
// the call to 'foo'.
//
proc main() {
  var x = new R();
  writeln("before: ", x);
  foo(x);
  writeln("after: ", x);

  for (loc, ptr) in pointers do on loc do deallocate(ptr);
}
