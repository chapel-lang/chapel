class MyClass {
  var x:int;
}
proc f(arg:shared MyClass) {
  writeln(arg);
}

f(nil);

var b:shared MyClass = nil;
writeln(b);
