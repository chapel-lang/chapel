// Modified from
// test/classes/constructors/nested-destructor.chpl
record R
{
  class C
  {
    proc init()
    {
      this.complete();
      writeln("    in init of C ", outer.id);
    }

    proc deinit()
    {
      writeln("    in deinit of C ", outer.id);
    }
  }

  var id : int;
  var c  : C;

  proc init()
  {
    // Note: calling 'new C()' in phase 1 will pass 'this' along, which is
    // generally frowned upon...
    this.complete();
    writeln("  in init of R");

    id = 1;
    c  = new C();
  }

  proc init(other:R)
  {
    writeln("  in copy initializer of R ", other.id);

    // TODO: maybe have way to allow the user to use
    // a compiler generated copy initializer?
    this.id = other.id + 1;
    this.c  = other.c;
  }

  proc deinit()
  {
    if id == 2 then
      writeln("  entered deinit of R ", id);
    else
      writeln("      entered deinit of R ", id);

    delete c;

    if id == 2 then
      writeln("  exiting deinit of R ", id);
    else
      writeln("      exiting deinit of R ", id);
  }
}

proc doit()
{
  writeln("doit entered");

  var x : R;

  writeln();
  writeln("  ", x);

  x.id = 2;

  writeln();
  writeln("  ", x);

  writeln();
  writeln("doit exiting");
}

doit();
