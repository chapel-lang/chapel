fun foo(type t) {
  var x : t;
  fun bar(_x) {
    writeln("default bar");
  }
  fun bar(_x : int) {
    writeln("bar of int");
  }
  bar(x);
  bar("hello world");
}

foo(string);
foo(int);
