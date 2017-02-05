class C {
  proc ~C() {
    writeln("In my destructor!");
  }
  
  proc deinit() {
    writeln("In my deinitializer!");
  }
}

var myC = new C();
delete myC;
