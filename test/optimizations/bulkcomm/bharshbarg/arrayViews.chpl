
use util;

proc main() {
  var A, B : [1..10, 1..10] int;
  ref RE = A.reindex(1..100 by 10, 1..100 by 10);
  write("Testing reindex...");
  stridedAssign(RE, B);
  stridedAssign(B, RE);
  writeln("OK");

  write("Testing slices of reindexes...");
  const firstThree = (1..100 by 10) # 3;
  stridedAssign(RE[firstThree, firstThree], B[1..3, 1..3]);
  stridedAssign(B[1..3, 1..3], RE[firstThree, firstThree]);
  writeln("OK");

  write("Testing reindex of reindex...");
  var MoreRE = RE.reindex(1..10, 1..10);
  stridedAssign(MoreRE, B);
  stridedAssign(B, MoreRE);
  writeln("OK");

  // rank changes
  write("Testing rank-change of reindex...");
  stridedAssign(RE[1,..], B[1,..]);
  stridedAssign(B[1,..], RE[1,..]);
  writeln("OK");

  ref rankChange = A[1,..];
  ref RCRE = rankChange.reindex(1..100 by 10);
  write("Testing reindex of rank-change...");
  stridedAssign(RCRE, B[1,..]);
  stridedAssign(B[1,..], RCRE);
  writeln("OK");
}
