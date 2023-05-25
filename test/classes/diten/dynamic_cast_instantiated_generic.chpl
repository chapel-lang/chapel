class C {
  type t;
  var a, b: t;
}

proc main {
  var c = (new owned C(int, 1, 2)).borrow();
  var o: object = c;
  writeln((o:c.type).a);
}
