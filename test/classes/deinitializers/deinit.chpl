class C {
  proc deinit() {
    writeln("In my deinitializer!");
  }
}

var myC = new unmanaged C();
delete myC;
