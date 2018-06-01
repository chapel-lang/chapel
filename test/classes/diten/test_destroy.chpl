class C {
  var a, b, c: int;
}

proc C.deinit() { writeln("In destructor"); }


proc main {
  var c: C = new unmanaged C(1,2,3);
  delete c;
}
