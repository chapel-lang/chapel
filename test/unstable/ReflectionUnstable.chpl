use Reflection;

record rec {
  var f = 0;
  proc foo() {}
  proc bar(x: int) {}
  proc type baz() {}
  proc type ding(x: int) {}
}

proc foo() {}
proc bar(x: int) {}

proc main() {
  use List;
  type T = list(int, ?);
  assert(!isFieldBound(T, "parSafe"));
  assert(!isFieldBound(T, 1));
  assert(canResolve("foo"));
  assert(canResolve("bar", 0));
  var r = new rec();
  assert(canResolveMethod(r, "foo"));
  assert(canResolveMethod(r, "bar", 0));
  assert(canResolveTypeMethod(rec, "baz"));
  assert(canResolveTypeMethod(rec, "ding", 0));
  getFieldRef(r, "f");
  getFieldRef(r, 0);
}
