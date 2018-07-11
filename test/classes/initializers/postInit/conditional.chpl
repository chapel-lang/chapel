
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
  var b : bool;
  proc init(b:bool) {
    writeln("B.init");
    this.b = b;
  }
  proc postinit() {
    if b {
      writeln("B.postinit");
      super.postinit();
    } else {
      super.postinit();
      writeln("B.postinit");
    }
  }
}

var t = new B(true);
delete t;

writeln();

var f = new B(false);
delete f;
