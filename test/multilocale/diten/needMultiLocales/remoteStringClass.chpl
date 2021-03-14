class C {
  var s: string;
}

proc main {
  var c = new borrowed C("a string");
  var b: int;
  writeln(c);
  on Locales(1) {
    b = c.s.byte(0);
    c.s = "another string";
  }
  writeln(b);
  writeln(c);
}
