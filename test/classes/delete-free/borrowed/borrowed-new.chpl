class MyClass {
  var x:int;
  proc init(arg:int) {
    this.x = arg;
  }
  proc deinit() {
    writeln("in MyClass.deinit");
  }
}

proc testa() {
  writeln("testa");
  var tmp = new borrowed MyClass(1);
  writeln(tmp);
  writeln(tmp.x);
  // tmp should be deleted
}

proc testb() {
  writeln("testb");
  var tmp = new unmanaged MyClass(1);
  writeln(tmp);
  writeln(tmp.x);
  delete tmp;
}

testa();
testb();
