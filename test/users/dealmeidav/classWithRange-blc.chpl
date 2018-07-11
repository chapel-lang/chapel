class A
{
  var rng: range(int);
}

proc main()
{
  var a = new unmanaged A(0..4);

  writeln(a.rng);

  delete a;
}
