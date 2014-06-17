class C {
  
}

class D : C {
}

proc writeType(c:C) {
  writeln("I am a C");
}

proc writeType(d:D) {
  writeln("I am a D");
}

proc foo(cOrD:C) {
  writeType(cOrD);
}

var myD = new D();

foo(myD);

delete myD;
