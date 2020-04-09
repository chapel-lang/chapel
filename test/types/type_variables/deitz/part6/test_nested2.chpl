proc foo(type t) {
  var x : t;
  proc bar(_x) {
    writeln("default bar");
  }
  proc bar(_x : int) {
    writeln("bar of int");
  }
  bar(x);
  bar("hello world");
}

foo(string);
foo(int);
