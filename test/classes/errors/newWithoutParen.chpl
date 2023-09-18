// modified from https://github.com/chapel-lang/chapel/issues/13201
// removed R1, as it does not work today due to other reasons and is not
// central to demonstrating this issue

class C {
  var x: int = 0;
}

class D {
  type T;
  var x: T;
}

record R2 {
  type T;
  var x: T;

  proc postinit() {
    // expect: false
    writeln(test1());
  }
  proc test1(): bool {
    x = new T; // should not compile due to this
    return x == nil;
  }
}

proc main() {
  var c2 = new R2(owned C?);
  var d2 = new R2(owned D(int)?);

  writeln("This code should not compile.");
}
