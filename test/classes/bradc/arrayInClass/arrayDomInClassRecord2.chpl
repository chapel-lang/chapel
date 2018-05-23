record R {
  var D = {1..10};
  var A: [D] real = [i in D] i;
}


class C {
  var D = {1..10};
  var A: [D] real = [i in D] i;
}



var myR = new R();
var myC = new borrowed C();

//baz(myR, 19); -- illegal, but not checked: can't modify const record field
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
