// Modified from
// test/classes/constructors/nested-destructor.chpl
record R
{
  class C
  {
    proc deinit()
    {
      writeln("in deinit of C");
    }
  }
  var c : C;

  proc init()
  {
    writeln("in init of R");
    c = new C();
  }

  proc deinit()
  {
    writeln("in deinit of R");
    delete c;
  }
}

proc doit() {
  var x: R;
}

doit();
