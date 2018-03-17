
class A {
  var a : int;

  proc init() {
    writeln("A.init");
  }

  proc postInit() {
    writeln("A.postInit");
  }
}

class B : A {
  var b : int;

  proc init() {
    writeln("B.init");
  }

  proc postInit() {
    writeln("B.postInit");
  }
}

class C : B {
  var c : int;

  proc init() {
    writeln("C.init");
  }

  proc postInit() {
    writeln("C.postInit");
  }
}

var c = new C();
delete c;
