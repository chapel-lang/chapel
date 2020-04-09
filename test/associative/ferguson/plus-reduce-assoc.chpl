// Can I use + reduce on associative domain?
var D: domain(int);

proc makeD(x:int) {
  var ret:domain(int);
  ret += x;
  return ret;
}

D = + reduce [i in 1..10] makeD(i);

writeln(D.sorted());

var D2 = + reduce for i in 1..10 do makeD(i);

writeln(D2.sorted());
