proc f() {
  @functionStatic
  var x = 0;
  x += 1;
  return x;
}
writeln(f());
writeln(f());
writeln(f());
