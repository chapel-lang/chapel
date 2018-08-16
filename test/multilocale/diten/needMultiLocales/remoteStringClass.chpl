class C {
  var s: string;
}

proc main {
  var c = new borrowed C("a string");
  var b: int;
  writeln(c);
  on Locales(1) {
    b = ascii(c.s);
    c.s = "another string";
  }
  writeln(b);
  writeln(c);
}
