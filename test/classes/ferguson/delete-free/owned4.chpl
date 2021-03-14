class C {
  var x = 1;
  proc deinit() {
    writeln("Destroying C ", x);
  }
}

proc foo() {
  var x:owned C?;
  var y = new owned C(2);

  x.retain(y.release());
}


foo();

