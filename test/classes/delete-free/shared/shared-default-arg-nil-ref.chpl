// test inspired by datetime.init

class MyClass {
  var x:int;
}

proc foo(arg:int, ref opt: shared MyClass? = nil ) {
  writeln(arg, " ", opt);
}

var s:shared MyClass? = new shared MyClass(3);

foo(1);
foo(2, s);

