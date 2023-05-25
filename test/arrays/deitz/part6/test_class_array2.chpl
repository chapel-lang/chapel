class C {
  var x: [1..3] int;
}

var a: [1..3] int;
var c = (new owned C(a)).borrow();
a(2) = 1;
writeln(c);
writeln(a);
