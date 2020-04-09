record R {
  var D: domain(1);
  var A: [D] real = [i in D] i;
}


class C {
  var D: domain(1);
  var A: [D] real = [i in D] i;
}


var myR = new R({1..10});
var myC = new owned C({1..10});

baz(myR, 19);
writeln("myR is: ", myR);
baz(myC, 20);
writeln("myC is: ", myC);


proc foo(x,y) {
  x(1) = y;
}

proc bar((x,y)) {
  x(1) = y;
}

proc baz(x,y) {
  x.A(1) = y;
}
