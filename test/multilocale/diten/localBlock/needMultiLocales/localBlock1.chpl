class C {
  var a: int;
  var b: real;
}

proc main {
  var c = new C(1, 1.0);

  on Locales(1) {
    c = new C(2, 2.0);
  }

  local {
    c = new C(4, 4.0);
  }
  writeln(c);
}
