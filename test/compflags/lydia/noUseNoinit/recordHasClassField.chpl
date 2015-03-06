class aClass {
  var namingIsHard: bool;
}

record foo {
  var t: aClass;

  proc foo() {
    writeln("I default initialized!");
  }
}
var bar: foo = noinit;
bar.t = new aClass();
writeln(bar);
