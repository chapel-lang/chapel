record R {
  var x;

  proc return_ref() const ref
  {
    return x;
  }
}

{
  var r = new R(1);

  writeln("setting it to 2");
  r.return_ref() = 2; // compilation error
}

