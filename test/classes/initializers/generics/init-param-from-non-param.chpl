class C {
  param p:int;
  proc init() {
    var notaparam = 77;
    p = notaparam;
    super.init();
  }
}

var c = new C();
writeln(c);
delete c;
