class C {
  var x = 1;
  var y = 2;
  var z = 3;

  iter these() {
    writeln("Calling serial iterator");
    yield x;
    yield y;
    yield z;
  }
}
var myCobj = new C();
var myC = myCobj.borrow();

for c in myC do
  writeln(c);

proc foo(x: int) {
  writeln(x);
}

writeln("Trying to promote foo():");
foo(myC);
