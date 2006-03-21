class C {
  type t;
  var x = 1;
  var y : t;
  function foo() {
    function bar(_x) {
      writeln("default bar");
    }
    function bar(_x : string) {
      writeln("bar of string");
    }
    bar(x);
    bar("hello world");
  }
}

var c = C(int);
c.foo();
writeln(c);
