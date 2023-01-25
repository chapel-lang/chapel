class Inner {
  var x: int;

  proc init(xVal: int) throws {
    x = xVal;
    this.complete();
    throw new Error();
  }
}

class Outer {
  var x: Inner;

  proc init(val: int) throws {
    x = new Inner(val); // Should not be allowed yet
  }
}

proc main() {
  var a = new Outer(10);
  writeln(a);
}
