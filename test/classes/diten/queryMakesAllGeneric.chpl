record R {
  param a = 1;
  param b = 2;
  param c = 3;
  type t = int;
}

proc foo(r: R(?b)) {
  var n: r.t;
  writeln((r.a, b, r.b, r.c, n));
}

proc bar(r: R) {
  writeln((r.a, r.b, r.c));
}

var a: R(c=4, t=real);

foo(a); // Because the value of the b field was queried, all 3 parameter fields
        // are generic, so the non-queried c field can have different values.

// bar(a); // Not allowed. bar's argument is not generic because the type R
           // has all parameters assigned and no fields are queried at the
           // formal argument.
