// regression test for https://github.com/chapel-lang/chapel/issues/23303.
record R {
  const D: range = 1..10;
}
writeln(new R());
