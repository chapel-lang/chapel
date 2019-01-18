
proc intFn(param k: int) {
  writeln("int(", k, ")");
  var s1 = "  127  ": int(k);
  var s2 = "  1_2_7 ": int(k);
  var s3 = "  127_ ": int(k);
  writeln((s1, s2, s3));
}

proc uintFn(param k: int) {
  writeln("uint(", k, ")");
  var s1 = "  127  ": uint(k);
  var s2 = "  1_2_7 ": uint(k);
  var s3 = "  127_ ": uint(k);
  writeln((s1, s2, s3));
}

for param i in 3..6 {
  intFn(2**i);
  uintFn(2**i);
}
