record MyRecord {
  param p: int;
  var r: range(?);

  proc init(param p:int, r) {
    this.p = p;
    this.r = r;
  }
}

var strided = 1..10 by 2;
var unstrided = 1..10;

var x = new MyRecord(1, unstrided);
writeln(x, ": ", x.type:string);

var y = new MyRecord(1, strided);
writeln(y, ": ", y.type:string);
