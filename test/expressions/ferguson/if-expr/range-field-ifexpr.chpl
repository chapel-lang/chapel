proc outputIt(y:int) {
  if y == 1 then
    writeln("OK");
  else
    writeln("BAD");
}

proc const range.testIfExpr(other:range(?)) {

  var lo1 = if hasLowBound() then this._low else other._low;
  outputIt(lo1);
}

proc main() {
  var a=1..10;
  var b=1..10;

  a.testIfExpr(b);
}
