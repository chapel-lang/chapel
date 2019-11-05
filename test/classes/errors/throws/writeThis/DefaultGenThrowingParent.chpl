//
// In this case, the compiler should resolve `writeThis` calls to class A's
// overload of `writeThis` in all three instances, because compiler
// generated `writeThis` routines are marked as "last resort" (?).
//

class A {
  var x: int = 0;

  proc writeThis(ch: channel) throws {
    ch <~> "Hello from A!";
  }
}

class A1: A {
  var y: int = 0;
}

proc main() {
  var a: A = new A();
  var b: A1 = new A1();
  var c: A = new A1();

  writeln(a);
  writeln(b);
  writeln(c);
}

