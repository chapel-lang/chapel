

class MyClass {
  var x:int;
}

proc acceptRef(ref arg:borrowed MyClass) {
  var other = new owned MyClass(2);
  arg = other;
}

proc test() {
  var x = new owned MyClass(1);

  acceptRef(x.borrow());

  writeln(x);
}

test();
