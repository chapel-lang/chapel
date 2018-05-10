class GenericClass {
  var f:integral;
  proc init(arg:integral) {
    this.f = arg;
  }
}


proc test() {
  var x = new GenericClass(1);
  var y:GenericClass = new GenericClass(2);

  writeln(x.type:string, " ", x);
  writeln(y.type:string, " ", y);
}

test();
