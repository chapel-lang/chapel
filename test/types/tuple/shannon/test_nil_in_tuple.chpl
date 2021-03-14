class C {
  var x : int;
}

var c = new unmanaged C();
var n : borrowed C?;
var t = (c, n);
delete c;
