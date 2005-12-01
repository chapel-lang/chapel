class C {
  var x : integer = 2;
  function foo() {
    var y : integer = x;
    writeln(y);
    function bar() {
      writeln(x + y);
    }
    bar();
  }
}

var c : C = C();

writeln(c);
c.foo();
