class Foo {
  type T;
  var x: T;

  proc init(param is_int: bool) {
    writeln("init(param)");
    T = if is_int then int else real;
    x = if is_int then 1 else 2;
  }

  proc init(val: ?T) {
    writeln("init(?T)");
    this.T = T;
    this.x = val;
  }

  proc init(param is_int: bool, val: ?T) {
    writeln("init(param, ?T)");
    // Problem was here: unresolved call
    // Compiler was incorrectly comparing method token and 'this' types
    this.init(is_int);
    this.x = val;
  }

  override proc writeThis(f) throws {
    f.write(T:string, " x=", x);
  }
}

proc main() {
  var f1 = new owned Foo(true);
  writeln("\t", f1); // int(64) x=1
  var f2 = new owned Foo(3:int(32));
  writeln("\t", f2); // int(32) x=3
  var f3 = new owned Foo(true, 4);
  writeln("\t", f3); // int(64) x=4
}
