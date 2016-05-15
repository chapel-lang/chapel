// Can I use + reduce on associative domain?
var D: domain(int);

proc makeD(x:int) {
  var ret:domain(int);
  ret += x;
  return ret;
}

D = + reduce [i in 1..10] makeD(i);

writeln(D.sorted());
