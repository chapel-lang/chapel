class C { var x: int; }

proc main() {
  var D2: domain(owned C);
  D2 += new owned C(1);
  D2 += new owned C(2);
  writeln(D2);
}
