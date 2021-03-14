use driver_domains;

record R {
  var r: int;
}

var A1D: [Dom1D] R;
var A2D: [Dom2D] R;
var A3D: [Dom3D] R;
var A4D: [Dom4D] R;
var A2D32: [Dom2D32] R;

on (Locales(numLocales-1)) do {
  local {
    A1D(n1).r = n1;
    A2D(n2,n2).r = n2;
    A3D(n3,n3,n3).r = n3;
    A4D(n4,n4,n4,n4).r = n4;
    A2D32(n5,n5).r = o5;
  }
 }

writeln(A1D(n1));
writeln(A2D(n2,n2));
writeln(A3D(n3,n3,n3));
writeln(A4D(n4,n4,n4,n4));
writeln(A2D32(n5,n5));
