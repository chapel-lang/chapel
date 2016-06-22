record R {
  param a = 1;
}

proc main {
  var a: R;
  begin writeln("(a = ", a.a, ")");
}

