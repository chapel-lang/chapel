record r1 {
  type t;
  type tt;
}

type t1 = r1(int);
var x: t1(real);

class C {
  param p1;
  param p2;
}

type t2 = unmanaged C(1)?;
var y: t2(2);

record r3 {
  var x;
  var y;
}

type t3 = r3(real);
var z: t3(int);


record r4 {
  type tt;
  param p;
  type t;
  var element: t;      // should not be included in warning
  var otherElement: t; // should not be included in warning
  var noType;
  type t2;
  param p2;
}

type t4 = r4(tt=int);

record r5 {
  type tt;
  var x: integral;
  var y: x.type;   // should not be included in warning
}

type t5 = r5(tt=int);
