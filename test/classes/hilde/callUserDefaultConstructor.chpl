// To show that a user-defined default-constructor is called.
//

class C
{
  proc init() { writeln("Called C()."); }
  proc deinit() { writeln("Called ~C()."); }
}

var c = new unmanaged C();
delete c;

writeln("Done.");
