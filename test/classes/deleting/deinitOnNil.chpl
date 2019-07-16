class C {
  proc deinit() {
    writeln("In C's deinit()");
  }
}

var myC: unmanaged C?;
delete myC;
