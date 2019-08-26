// test inspired by datetime.init

class MyClass {
  var x:int;
}

proc foo(arg:int, in opt: shared MyClass? = nil ) {
  writeln(arg, " ", opt);
}

foo(1);
foo(2, new shared MyClass(3));
