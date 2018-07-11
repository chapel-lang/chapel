class aClass {
  var namingIsHard: bool;
}

record foo {
  var t: unmanaged aClass;

  proc foo() {
    writeln("I default initialized!");
  }
}

var bar: foo = noinit;

bar.t = new unmanaged aClass();

writeln(bar);

delete bar.t;

