class C {
  proc deinit(x=1) {
    writeln("In my deinitializer, x is ", x, "!");
  }
}

var myC = new C();
myC.deinit(2);
delete myC;
