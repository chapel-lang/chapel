class A {
  var a: int;

  proc init(aVal) {
    a = aVal;
  }
}

class B : A {
  var b = false;

  proc init(bVal, aVal) {
    super.init(aVal);
    b = bVal;
  }
}

class C : B {
  var c = 4.7;

  proc init(cVal, bVal, aVal) {
    super.init(bVal, aVal);
    c = cVal;
    a = aVal;
  }
}

var c = new unmanaged C(6.2, true, 3);
writeln(c);
delete c;
