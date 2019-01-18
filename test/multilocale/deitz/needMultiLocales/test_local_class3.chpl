class D {
  var x = 17;
}

class C {
  var d = new unmanaged D();
}

proc main {
  var c = new unmanaged C();
  writeln(c);
  on Locales(1) {
    delete c.d;
    c.d = nil; 
  }
  writeln(c);

  delete c;
}
