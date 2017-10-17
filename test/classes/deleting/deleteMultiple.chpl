class C {
  var x;

  proc deinit() {
    writeln("Deleting a C: ", x);
  }
}

var x = new C(1),
    y = new C(2);

delete x, y;
