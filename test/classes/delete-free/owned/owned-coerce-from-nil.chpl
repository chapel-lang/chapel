class MyClass {
  var x:int;
}
proc f(arg:owned MyClass) {
  writeln(arg);
}

f(nil);

var b:owned MyClass = nil;
writeln(b);
