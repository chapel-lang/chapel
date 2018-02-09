config var notaparam = 77;

class C {
  param p:int;
  proc init() {
    p = notaparam;
    super.init();
  }
}

var c = new C();
writeln(c);
delete c;
