class C { var x:int; }

proc badForExpr() {
  var x = for i in 1..3 do new borrowed C(i);
  writeln(x);
}
badForExpr();
