def foo(type t) {
  var x : t;
  def bar(_x) {
    writeln("default bar");
  }
  def bar(_x : int) {
    writeln("bar of int");
  }
  bar(x);
  bar("hello world");
}

foo(string);
foo(int);
