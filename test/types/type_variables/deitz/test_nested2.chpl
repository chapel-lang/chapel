function foo(type t) {
  var x : t;
  function bar(_x) {
    writeln("default bar");
  }
  function bar(_x : int) {
    writeln("bar of int");
  }
  bar(x);
  bar("hello world");
}

foo(string);
foo(int);
