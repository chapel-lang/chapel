class C {
  proc deinit() {
    writeln("Destroying C");
  }
}

proc foo() {
  var x:owned C?;
  var y = new owned C();

  x = y;
}


foo();

