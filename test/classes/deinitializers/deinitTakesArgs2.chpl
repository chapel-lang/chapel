class C {
  proc deinit() {
    writeln("In my deinitializer!");
  }

  proc deinit(x:int) {
    writeln("In my deinitializer, x is ", x, "!");
  }
}

var myC = new C();
myC.deinit(2);
myC.deinit();
delete myC;
