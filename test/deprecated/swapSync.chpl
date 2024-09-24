{
  var a: int = 1;
  var b: sync int = 2;
  a <=> b;
  writeln((a, b.readFF()));
}
{
  var a: sync int = 1;
  var b: int = 2;
  a <=> b;
  writeln((a.readFF(), b));
}
{
  var a: sync int = 1;
  var b: sync int = 2;
  a <=> b;
  writeln((a.readFF(), b.readFF()));
}
