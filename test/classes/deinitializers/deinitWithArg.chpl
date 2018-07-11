class C {
  proc deinit(x: int) {
    writeln("In deinit, x is: ", x, "!");
  }
}

var myC = new unmanaged C();
delete myC;
