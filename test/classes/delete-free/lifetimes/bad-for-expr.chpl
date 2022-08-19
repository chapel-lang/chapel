class C { var x:int; }

proc badForExpr() {
  var x = for i in 1..3 do (new owned C(i)).borrow();
  writeln(x);
}
badForExpr();
