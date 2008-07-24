record R {
  var A: [i in 1..10] real = i;
}

var myR = new R();

writeln("myR is: ", myR);
baz(myR, 19);
writeln("myR is: ", myR);

def baz(inout x,y) {
  x.A(1) = y;
}
