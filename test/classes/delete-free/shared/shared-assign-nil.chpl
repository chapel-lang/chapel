class MyClass {
  var x:int;
}


proc test() {
  var s:shared MyClass? = nil;
  s = nil;

  writeln(s);
}

test();
