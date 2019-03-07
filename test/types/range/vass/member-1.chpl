const rbase = .. by 3;
for al in 0..3 {
  const r = rbase align al;
  writeln("r = ", r);
  for i in 0..3 {
    writeln("r.contains(", i, ") = ", r.contains(i));
  }
}
