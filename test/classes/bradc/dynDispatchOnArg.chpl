class C {
  
}

class D : C {
}

def writeType(c:C) {
  writeln("I am a C");
}

def writeType(d:D) {
  writeln("I am a D");
}

def foo(cOrD:C) {
  writeType(cOrD);
}

var myD = new D();

foo(myD);
