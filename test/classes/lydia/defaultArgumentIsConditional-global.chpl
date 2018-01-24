var global = true;

record Foo {
  param stridable:bool;
  var _stride;

  proc someFunc(arg2 = if global then 1 else 2) {
    writeln(arg2);
  }
}

proc main() {
  var f = new Foo(true, 3);
  f.someFunc();
}
