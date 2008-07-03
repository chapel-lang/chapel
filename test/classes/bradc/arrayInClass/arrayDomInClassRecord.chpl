record R {
  var D: domain(1);
  var A: [i in D] real = i;
}


class C {
  var D: domain(1);
  var A: [i in D] real = i;
}



var myR = new R([1..10]);
var myC = new C([1..10]);

baz(myR, 19);
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
