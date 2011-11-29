class CC {
  type t;
  var d : t;
}

record R {
  type Rtype;
  var  data: Rtype;
  var  valid: bool;
}

class C {
  type ind_type;
  var inds: CC(R(ind_type));
}

var c: C(real) = new C(real);
delete c;
