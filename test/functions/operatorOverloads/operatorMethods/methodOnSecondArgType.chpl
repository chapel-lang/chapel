// Ensures that it doesn't matter which arg you associate the operator method
// with, we'll still find the operator
record Rec {
  var x: int;

  operator +(lhs: int, rhs: Rec) {
    return lhs + rhs.x;
  }
}

proc main() {
  var lhs = 3;
  var rhs = new Rec(4);
  var res = lhs + rhs;
  writeln(res.type: string);
  writeln(res);
}
