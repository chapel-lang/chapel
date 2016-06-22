class C
{
  proc writeThis(w) { w.write("C"); }
}

class SubC : C
{
  proc writeThis(w) { w.write("SubC"); }
}

class OverrideMe
{
  proc getC()
  {
    return new C();
  }

  iter manyC()
  {
    yield new C();
    yield new C();
  }
}

class OverridesIt : OverrideMe
{
  proc getC()
  {
    return new SubC();
  }

  iter manyC()
  {
    yield new SubC();
    yield new SubC();
  }
}

proc main()
{
  var o : OverrideMe;

  o = new OverridesIt();

  var t1 = o.getC();

  writeln("Should be subC: ", t1);

  delete t1;


  writeln("Many: ", o.manyC());

  delete o;
}
