// To show that a user-defined default-initializer is called.
//

class C
{
  proc init() { writeln("Called C.init()."); }
  proc deinit() { writeln("Called C.deinit()."); }
}

var c = new C();
delete c;

writeln("Done.");
