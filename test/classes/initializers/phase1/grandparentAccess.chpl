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
  var c = 4;

  proc init(cVal, bVal, aVal) {
    // Ensure that the compiler issues an error for accessing 'a' before it is
    // initialized.
    var x = a;
    super.init(bVal, aVal);
    c = cVal;
  }
}

var c = new C(6, true, 3);
writeln(c);
