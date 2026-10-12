// reproducer for https://github.com/chapel-lang/chapel/issues/28675
proc foo(const ref bias,OC) {
  for o in 0..<OC {
    var val = if bias.type!=nil.type then bias[o] else 0.0:real(32);
    writeln(val);
  }
}
proc main() {
  const OC = 5;
  foo(nil, OC);
}
