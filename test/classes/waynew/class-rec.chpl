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
  var inds: unmanaged CC(R(ind_type))?;
}

var c: unmanaged C(real) = new unmanaged C(real);
delete c;
