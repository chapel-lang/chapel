
class C {
  var c : int;

  proc init() {
    writeln("C.init");
  }

  proc postInit() {
    writeln("C.postInit");
  }
}

var c = new C();
c.postInit();
delete c;
