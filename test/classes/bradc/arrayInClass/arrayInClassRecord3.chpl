var A: [1..10] real = [i in 1..10] i;

record R {
  var A: [1..10] real = 1..10;
}


class C {
  var A: [1..10] real = 1..10;
}



var myR = new R();
var myC = new unmanaged C();

writeln("A is: ", A);
foo(A, 17);
writeln("A is: ", A);
bar((A, 18));
writeln("A is: ", A);
//baz(myR, 19);  -- illegal, but not checked: can't modify const record field
writeln("myR is: ", myR);
baz(myC, 20);
writeln("myC is: ", myC);

delete myC;

proc foo(x,y) {
  x(1) = y;
}

proc bar((x,y)) {
  x(1) = y;
}

proc baz(x,y) {
  x.A(1) = y;
}
