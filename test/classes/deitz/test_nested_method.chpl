class C {
  var x : int = 2;
  function foo() {
    var y : int = x;
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
