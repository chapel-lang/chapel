class aClass {
  var namingIsHard: bool;
}

record foo {
  var t: unmanaged aClass?;
}

var bar: foo = noinit;

bar.t = new unmanaged aClass();

writeln(bar);

delete bar.t;
