class C {
  proc deinit() {
    writeln("In C's deinit()");
  }
}

var myC: C;
delete myC;
