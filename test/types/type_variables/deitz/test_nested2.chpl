function foo(type t) {
  var x : t;
  function bar(_x) {
    writeln("default bar");
  }
  function bar(_x : integer) {
    writeln("bar of integer");
  }
  bar(x);
  bar("hello world");
}

foo(string);
foo(integer);
