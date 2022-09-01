class C {
  var x = 2;
  proc foo return 3;
  proc bar return foo;
}

var c = (new owned C(4)).borrow();

writeln(c.bar);
