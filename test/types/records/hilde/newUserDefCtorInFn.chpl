// To show that a user-defined default-constructor is called by an explicit
// (record) new.
//

record R
{
  proc init() { writeln("Called R()."); }
  proc deinit() { writeln("Called ~R()."); }
}

// Different behavior is expected, depending on whether the variable is
// allocated in function vs. module scope (so we have to test both).
proc foo()
{
  var r:R = new R();
}

foo();

writeln("Done.");
