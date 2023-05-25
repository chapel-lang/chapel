class GenericClass {
  var f:integral;
  proc init(arg:integral) {
    this.f = arg;
  }
}


proc test() {
  var x = (new owned GenericClass(1)).borrow();
  var y:borrowed GenericClass = (new owned GenericClass(2)).borrow();
  var z:borrowed GenericClass(int) = (new owned GenericClass(2)).borrow();

  writeln(x.type:string, " ", x);
  writeln(y.type:string, " ", y);
}

test();
