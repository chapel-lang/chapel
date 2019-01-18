class C {
  proc ~C() {
    writeln("In my destructor!");
  }
  
  proc deinit() {
    writeln("In my deinitializer!");
  }
}

var myC = new unmanaged C();
delete myC;
