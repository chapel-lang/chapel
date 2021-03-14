proc writeme(r:range(?)) where isBoundedRange(r) {
  write(r.low, "..", r.high);
  if r.stridable {
    write(" by ", r.stride);
    if r.aligned then write(" align ", r.alignment);
    else              write(" align ?");
  }
}

proc test(r:range(?), offs:r.idxType) {
  writeme(r);
  const res = r.offset(offs);
  write("  offs ", offs, "  ");
  writeme(res);
  const offs2 = if r.stridable then offs else 0;
  if !res.aligned || res.alignment != r.first + offs2 then
    write(" ***ERROR***");
  writeln();
}

proc suite(r:range(?)) {
  for i in -4 .. 4 by 2 do
    test(r, i);
}

proc supersuite(r:range) {
  const steps = (1, 2, -1, -2);
  suite(r);
  for param i in 0..steps.size-1 do suite(r by steps(i));
}

// no first index: supersuite(1..0);
supersuite(3..3);
supersuite(1..2);
supersuite(1..6);
supersuite(-5..-1);
supersuite(-5..5);
