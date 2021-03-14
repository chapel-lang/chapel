class C {
  var x;

  proc deinit() {
    writeln("Deleting a C: ", x);
  }
}

var x = new unmanaged C(1),
    y = new unmanaged C(2);

delete x, y;
