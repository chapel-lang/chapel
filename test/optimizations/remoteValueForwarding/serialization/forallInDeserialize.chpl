
record R {
  const n : int;
  var A : [1..n] int;
}

proc R.chpl__serialize() {
  return n;
}

proc type R.chpl__deserialize(in n) {
  var ret = new R(n);
  forall a in ret.A do a = 1;
  return ret;
}

proc main() {
  var rr = new R(3);

  const r = rr;

  coforall loc in Locales do on loc {
    writeln(r.A.first);
  }
}
