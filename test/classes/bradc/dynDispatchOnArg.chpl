class C {
  
}

class D : C {
}

proc writeType(c: borrowed C) {
  writeln("I am a C");
}

proc writeType(d: borrowed D) {
  writeln("I am a D");
}

proc foo(cOrD: borrowed C) {
  writeType(cOrD);
}

var myD = new unmanaged D();

foo(myD);

delete myD;
