record r {
  type t;
  type tt;
}

type t = r(int);
var x: t(real);

class C {
  param p1;
  param p2;
}

type tt = unmanaged C(1)?;
var y: tt(2);
