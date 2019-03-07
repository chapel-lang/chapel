class aClass {
  var namingIsHard: bool;
}

record foo {
  var t: unmanaged aClass;

  proc init() {
    writeln("I default initialized!");
  }
}

var bar: foo = noinit;

bar.t = new unmanaged aClass();

writeln(bar);

delete bar.t;

