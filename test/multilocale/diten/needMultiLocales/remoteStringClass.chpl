class C {
  var s: string;
}

proc main {
  var cObj = new owned C("a string");
  var c = cObj.borrow();
  var b: int;
  writeln(c);
  on Locales(1) {
    b = c.s.byte(0);
    c.s = "another string";
  }
  writeln(b);
  writeln(c);
}
