// To show that a user-defined default-constructor is called.
//

class C
{
  proc C() { writeln("Called C()."); }
  proc deinit() { writeln("Called ~C()."); }
}

var c = new C();
delete c;

writeln("Done.");
