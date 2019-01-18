class C
{
  proc writeThis(w) { w.write("C"); }
}

class SubC : C
{
  override proc writeThis(w) { w.write("SubC"); }
}

class OverrideMe
{
  proc getC()
  {
    return new unmanaged C();
  }

  iter manyC()
  {
    yield new unmanaged C();
    yield new unmanaged C();
  }
}

class OverridesIt : OverrideMe
{
  override proc getC()
  {
    return new unmanaged SubC();
  }

  override iter manyC()
  {
    yield new unmanaged SubC();
    yield new unmanaged SubC();
  }
}

proc main()
{
  var o : unmanaged OverrideMe;

  o = new unmanaged OverridesIt();

  var t1 = o.getC();

  writeln("Should be subC: ", t1);

  delete t1;


  writeln("Many: ", o.manyC());

  delete o;
}
