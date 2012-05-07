record myrec {
  var x:int;
}

proc _chpl_swap(a:myrec, b:myrec) {
  writeln("in swap ", a, " ", b);
}

var a = new myrec(1);
var b = new myrec(2);

a <=> b;
new myrec(1) <=> new myrec(2);
new myrec(1) <=> new myrec(2) <=> new myrec(3);

