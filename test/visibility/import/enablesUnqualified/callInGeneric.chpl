proc foo() {
  writeln('foo');
}

proc test(param x=1) {
  import Reflection.canResolve;
  if canResolve('foo') then
    foo();
}
test();
