
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
    writeln("Start of B.postinit");
    super.postinit();
    writeln("End of B.postinit");
  }
}

var b = new unmanaged B();
delete b;
