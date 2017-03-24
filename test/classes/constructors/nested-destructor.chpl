record R
{
  class C
  {
    proc deinit()
    {
      writeln("in ~C");
    }
  }
  var c : C;

  proc R()
  {
    writeln("in R()");
    c = new C();
  }

  proc deinit()
  {
    writeln("in ~R");
    delete c;
  }
}

proc doit() {
  var x: R;
}

doit();
