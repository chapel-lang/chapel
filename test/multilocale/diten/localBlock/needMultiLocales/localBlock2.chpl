class C {
  var a: int;
  var b: real;
}

proc main {
  var c = new unmanaged C(1, 1.0);

  on Locales(1) {
    c = new unmanaged C(2, 2.0);
  }

  local {
    c.a = 3; // error: c.a is remote
  }
  writeln(c);
}
