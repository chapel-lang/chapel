var x = 42;
iter foo(const y) ref {
  yield y;
}
for i in foo(x) do i = 43;
writeln(x);
