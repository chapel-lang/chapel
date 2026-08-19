union u {
  var x: int;
  var y: int;
  var z: real;
}

var u1 = new owned u();
var u2 = new shared u();
var u3 = new unmanaged u();
//var u4 = new borrowed u();

