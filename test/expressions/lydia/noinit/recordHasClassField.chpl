class aClass {
  var namingIsHard: bool;
}

record foo {
  var t: aClass;
}
var bar: foo = noinit;
bar.t = new aClass();
writeln(bar);
