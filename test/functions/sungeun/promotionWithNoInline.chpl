
record R {
  var t: int;
}

class C {
  var r: [-4..7] R;
}

var tmpC = new owned C();
tmpC.r.t = 99;
