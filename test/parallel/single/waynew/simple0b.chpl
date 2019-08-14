// Should not be able to assign to a single more than once.

class C {
  var x: int;
}

var s: single borrowed C?;
var t: borrowed C?;

s = new borrowed C();
s = t;


