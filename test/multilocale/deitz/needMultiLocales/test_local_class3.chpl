class D {
  var x = 17;
}

class C {
  var d = new D();
}

proc main {
  var c = new C();
  writeln(c);
  on Locales(1) {
    c.d = nil; 
  }
  writeln(c);
}
