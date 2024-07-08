// Support primitive type queries embedded in array type expressions.
proc foo(ref x: [?d] int(?w), n : int(w)) {
  for idx in d do x[idx] = n;
}

proc main() {
  var a: [0..4] int(8);
  writeln(a);
  foo(a, 8);
  writeln(a);
}

