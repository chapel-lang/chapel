record R {
  var A: [1..10] real = [i in 1..10] i;
}

var myR = new R();

writeln("myR is: ", myR);
baz(myR, 19);
writeln("myR is: ", myR);

proc baz(inout x,y) {
  x.A(1) = y;
}
