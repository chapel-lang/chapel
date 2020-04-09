class C {
  proc deinit() {
    writeln("Destroying C");
  }
}

proc foo() {
  var x = new owned C();

  var y = x;
}


foo();

