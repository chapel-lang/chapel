record R {
  var D = [1..10];
  var A: [i in D] real = i;
}


class C {
  var D = [1..10];
  var A: [i in D] real = i;
}



var myR = new R();
var myC = new C();

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
