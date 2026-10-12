// regression test for https://github.com/chapel-lang/chapel/issues/23355
proc f(): domain(?) {
  var d: domain(1);
  return d;
}
writeln(f());
proc g(): domain {
  var d: domain(1);
  return d;
}
writeln(g());
