class A {
  var a: int;

  proc init(aVal) {
    a = aVal;
    super.init();
  }
}

class B : A {
  var b = false;

  proc init(bVal, aVal) {
    b = bVal;
    super.init(aVal);
  }
}

class C : B {
  var c = 4.7;

  proc init(cVal, bVal, aVal) {
    c = cVal;
    a = aVal;
    super.init(bVal, aVal);
  }
}

var c = new C(6.2, true, 3);
writeln(c);
delete c;
