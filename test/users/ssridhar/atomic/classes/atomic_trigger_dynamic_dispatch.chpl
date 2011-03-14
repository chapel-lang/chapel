class C {
  proc x() {
    writeln("In C's x");
  }
}

class D : C {
  proc x() {
    writeln("In D's x");
  }
}

proc main {
  var c = new C(); 
  var d = new D(); 
  atomic c.x();
  d.x();
}