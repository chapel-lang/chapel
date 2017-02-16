class C {
  proc deinit() {
    writeln("In my deinitializer!");
  }
}

var myC = new C();
delete myC;
