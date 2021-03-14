
class A {
  var a : int;

  proc init() {
    writeln("A.init");
  }

  proc postinit() {
    writeln("A.postinit");
  }
}

class B : A {
  var b : int;

  proc init() {
    writeln("B.init");
  }

  proc postinit() {
    writeln("B.postinit");
  }
}

class C : B {
  var c : int;

  proc init() {
    writeln("C.init");
  }

  proc postinit() {
    writeln("C.postinit");
  }
}

var c = new shared C();
