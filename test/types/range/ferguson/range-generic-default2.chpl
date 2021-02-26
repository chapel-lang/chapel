record MyRecord {
  param p: int;
  var r: range;

  proc init(param p:int, r) {
    this.p = p;
    this.r = r;
  }
}

var strided = 1..10 by 2;
var unstrided = 1..10;

// expecting error here: 
// cannot initialize a non-stridable range from a stridable range
var y = new MyRecord(1, strided);
