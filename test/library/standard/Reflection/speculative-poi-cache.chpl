record Outermost {}

proc generic(x) {
  use Reflection;
  return canResolveMethod(x, "foo");
}

proc case1() {
  proc Outermost.foo() do return "Outer foo";
  return generic(new Outermost());
}

proc case2() {
  return generic(new Outermost());
}

proc main() {
  writeln(case1());
  writeln(case2());
}
