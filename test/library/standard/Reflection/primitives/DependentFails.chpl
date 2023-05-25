
record inner {
  proc num(): int { return 8; }
}

record r {
  var x = new inner();
  proc foo(): inner { return "inner"; }
}

proc test() {
  var obj = new r();
  param yes = __primitive("resolves", obj.foo().num());
  assert(!yes);
}
test();

