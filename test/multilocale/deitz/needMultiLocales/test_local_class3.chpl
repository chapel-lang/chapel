class D {
  var x = 17;
}

class C {
  var d = new D();
}

proc main {
  var c = new C();
  writeln(c);
  on rootLocale.getLocales()(1) {
    c.d = nil; 
  }
  writeln(c);
}
