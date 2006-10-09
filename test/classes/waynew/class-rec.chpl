record R {
  type Rtype;
  var  data: Rtype;
  var  valid: bool;
}

class C {
  type ind_type;
  var inds: _ddata(R(ind_type));
  // var inds: R(ind_type);
}


var c: C(float) = C(float);
