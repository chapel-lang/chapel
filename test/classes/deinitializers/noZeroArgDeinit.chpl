class C {
  proc deinit(x: int) {
    writeln("In deinit, x is: ", x, "!");
  }
}

var myC = new C();
myC.deinit(2);
delete myC;
