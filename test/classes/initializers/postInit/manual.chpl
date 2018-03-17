
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
    writeln("Start of B.postInit");
    super.postInit();
    writeln("End of B.postInit");
  }
}

var b = new B();
delete b;
