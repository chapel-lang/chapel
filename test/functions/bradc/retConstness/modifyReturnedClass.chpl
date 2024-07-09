class C { var x: int; }

var ownC1 = new owned C(1);
var c1 = ownC1.borrow();

proc foo() {
  return c1;
}

proc bar() {
  var c2 = new shared C(2);
  return c2;
}

writeln("foo().x = ", foo().x);
writeln("bar().x = ", bar().x);

writeln("trying to write into foo().x and bar().x...");
foo().x = 3;
bar().x = 4;
writeln("succeeded");

writeln("c1 is: ", c1);
writeln("foo().x = ", foo().x);
writeln("bar().x = ", bar().x);
