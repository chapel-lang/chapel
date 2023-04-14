class A
{
  var rng: range(int, boundKind.both, false);
}

proc main()
{
  var a = new unmanaged A(0..4);

  writeln(a.rng);

  delete a;
}
