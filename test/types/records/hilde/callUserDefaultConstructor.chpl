// To show that a user-defined default-constructor is called.
//

record R
{
  proc R() { writeln("Called R()."); }
  proc deinit() { writeln("Called ~R()."); }
}

var r:R;

writeln("Done.");
