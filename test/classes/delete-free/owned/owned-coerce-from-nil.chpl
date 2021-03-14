class MyClass {
  var x:int;
}
proc f(in arg:owned MyClass?) {
  writeln(arg);
}

f(nil);

var b:owned MyClass? = nil;
writeln(b);
