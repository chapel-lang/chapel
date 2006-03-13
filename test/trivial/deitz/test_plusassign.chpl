class D {
  var y = 2;
}

class C : D {
  var x = 1;
  function foo() {
    function bar(_x) {
      writeln("default bar");
    }
    function bar(_x : D) {
      writeln("bar of D");
    }
    bar(x);
    bar(this);
    x += 1;
  }
}

var d = D();
writeln(d);

var c = C();
c.foo();
writeln(c);
