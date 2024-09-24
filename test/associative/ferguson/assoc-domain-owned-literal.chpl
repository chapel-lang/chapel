class C { var x: int; }

proc main() {
  var D1 = {new owned C(1), new owned C(2)};
  writeln(D1);
}
