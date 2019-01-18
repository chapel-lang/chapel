record Foo {
  param stridable:bool;
  var _stride;

  proc someFunc(param arg1:bool = false,
                arg2 = if arg1 then 1 else 2) {
    writeln(arg2);
  }
}

proc main() {
  var f = new Foo(true, 3);
  f.someFunc();
}
