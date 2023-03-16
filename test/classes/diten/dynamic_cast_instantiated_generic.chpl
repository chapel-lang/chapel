class C {
  type t;
  var a, b: t;
}

proc main {
  var cOwn = new owned C(int, 1, 2);
  var c = cOwn.borrow();
  var o: object = c;
  writeln((o:c.type).a);
}
