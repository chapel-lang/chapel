
proc printType(const x) {
  writeln("type = ", x.type:string);
}

record R {
  var x : int;
  var y : real;

  proc init() {
    this.x = 1;
    this.y = 1.0;
    printType(this);
    this.complete();
  }
}
