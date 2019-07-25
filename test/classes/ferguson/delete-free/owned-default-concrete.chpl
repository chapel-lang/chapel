class MyClass {
  var x = 1;
  proc deinit() {
    writeln("Destroying C ", x);
  }
}

proc foo(x:owned MyClass?) {
  writeln("In foo, ", x);
}

proc test() {
  var x: owned MyClass? = new owned MyClass(1);
  writeln("In test x=", x.borrow());
  foo(x);
  writeln("In test again, x=", x.borrow());
}

test();
