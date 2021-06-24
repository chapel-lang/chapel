// remoteNestedRecordAssignment.chpl
//
// Demonstrates that nested record assignment is performed correctly including
// internode communication.
//
// According to the spec [16.9.1], "... each field of the record on the
// left-hand side of the assignment is overwritten with the value in the
// like-named field of the record on the right-hand side as if by assignment."
//
record R {
  var i:int;
}

proc R.init=(other:R) {
  this.i = other.i;
}

operator R.=(ref lhs:R, rhs:R) {
  lhs.i = rhs.i;
  writeln("overwrote lhs.i with ", rhs.i);
}

record S { var r:R; }

var s:S;
on Locales[numLocales-1] {
  var t:S;
  t.r.i = 121;
  s = t;
}
writeln(s);
