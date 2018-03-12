class A {
  var a: int;

  proc init(aVal) {
    a = aVal;
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
  var c = 4;

  proc init(cVal, bVal, aVal) {
    c = a;
    super.init(bVal, aVal);
    c += cVal;
  }
}

var c = new C(6, true, 3);
writeln(c);
delete c;
