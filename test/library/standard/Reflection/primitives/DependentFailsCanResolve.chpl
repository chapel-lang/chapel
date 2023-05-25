use Reflection;

record inner {
  proc num(): int { return 8; }
}

record r {
  var x = new inner();
  proc foo(): inner { return "inner"; }
}

proc test() {
  var obj = new r();
  param yes = canResolveMethod(obj, "foo");
  assert(!yes);
}
test();

