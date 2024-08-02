// test that swapping 2 syncs is an error
// this is currently only a deprecation, but should become an error
{
  var a: sync int = 1;
  var b: sync int = 2;
  a <=> b;
  writeln((a.readFF(), b.readFF()));
}


