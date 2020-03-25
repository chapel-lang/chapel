class C { }

proc error() {
  // These are just never mentioned again
  var a1;
  type t1;
  ref r1;
  var b1: borrowed C;
  var c1: C;
  var q1: C?;
  
  // These have invalid split inits
  var a2;
  type t2;
  ref r2;
  var b2: borrowed C;
  var c2: C;
  var q2: C?;

  writeln(a2);
  writeln(t2:string);
  writeln(r2);
  writeln(b2);
  writeln(c2);
  writeln(q2);

  a2 = 1;
  t2 = int;
  r2 = a2;
  b2 = (new owned C()).borrow();
  c2 = new owned C();
  q2 = new owned C();
}
error();

var b;
type tt;
ref rr;
var bb: borrowed C;
var cc: C;
var qq: C?;
