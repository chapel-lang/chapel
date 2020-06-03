class MyClass {
  var x = 1;
  proc deinit() {
    writeln("Destroying C ", x);
  }
}

proc foo(x) {
  writeln("In foo, ", x);
}

proc bar(x:owned) {
  writeln("In bar, ", x);
}

proc baz(type t, x:t) {
  writeln("In baz, ", x);
}

proc testa() {
  writeln("testing generic (unspecified) argument");
  var x = new owned MyClass(1);
  writeln("In testa x=", x.borrow());
  foo(x);
  writeln("In testa again, x=", x.borrow());
}

proc testb() {
  writeln("testing generic :owned argument");
  var x: owned MyClass? = new owned MyClass(1);
  writeln("In testb x=", x.borrow());
  bar(x);
  writeln("In testb again, x=", x.borrow());
}

proc testc() {
  writeln("testing generic type, arg pattern");
  var x: owned MyClass? = new owned MyClass(1);
  writeln("In testc x=", x.borrow());
  baz(x.type, x);
  writeln("In testc again, x=", x.borrow());
}

testa();
testb();
testc();
