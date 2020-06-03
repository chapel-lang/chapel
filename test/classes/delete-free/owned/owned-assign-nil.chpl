class MyClass {
  var x:int;
}


proc test() {
  var s:owned MyClass? = nil;
  s = nil;

  writeln(s);
}

test();
