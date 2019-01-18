// To show that a user-defined default-constructor is called by an explicit
// (record) new.
//

record R
{
  proc init() { writeln("Called R()."); }
  proc deinit() { writeln("Called ~R()."); }
}

var r:R = new R();

writeln("Done.");
