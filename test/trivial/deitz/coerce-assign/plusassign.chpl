class D {
  var y = 2;
}

class C : D {
  var x = 1;
  proc foo() {
    proc bar(_x) {
      writeln("default bar");
    }
    proc bar(_x : borrowed D) {
      writeln("bar of D");
    }
    bar(x);
    bar(this);
    x += 1;
  }
}

var d = new borrowed D();
writeln(d);

var c = new borrowed C();
c.foo();
writeln(c);
