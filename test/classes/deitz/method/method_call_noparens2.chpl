class C {
  var x = 2;
  proc foo do return 3;
  proc bar do return foo;
}

var c = (new owned C(4)).borrow();

writeln(c.bar);
