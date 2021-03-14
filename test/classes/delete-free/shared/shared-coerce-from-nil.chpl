class MyClass {
  var x:int;
}
proc f(in arg:shared MyClass?) {
  writeln(arg);
}

f(nil);

var b:shared MyClass? = nil;
writeln(b);
