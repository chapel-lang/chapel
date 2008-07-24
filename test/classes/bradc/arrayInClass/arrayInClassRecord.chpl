var A: [i in 1..10] real = i;

record R {
  var A: [i in 1..10] real = i;
}


class C {
  var A: [i in 1..10] real = i;
}



var myR = new R();
var myC = new C();

writeln("A is: ", A);
foo(A, 17);
writeln("A is: ", A);
bar((A, 18));
writeln("A is: ", A);
//baz(myR, 19); -- illegal, but not checked: can't modify const record field
writeln("myR is: ", myR);
baz(myC, 20);
writeln("myC is: ", myC);

def foo(x,y) {
  x(1) = y;
}

def bar((x,y)) {
  x(1) = y;
}

def baz(x,y) {
  x.A(1) = y;
}
