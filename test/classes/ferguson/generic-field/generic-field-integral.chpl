class GenericClass {
  var f:integral;
  proc init(arg:integral) {
    this.f = arg;
  }
}


proc test() {
  var ownX = new owned GenericClass(1);
  var x = ownX.borrow();
  var ownY = new owned GenericClass(2);
  var y:borrowed GenericClass = ownY.borrow();
  var ownZ = new owned GenericClass(2);
  var z:borrowed GenericClass(int) = ownZ.borrow();

  writeln(x.type:string, " ", x);
  writeln(y.type:string, " ", y);
}

test();
