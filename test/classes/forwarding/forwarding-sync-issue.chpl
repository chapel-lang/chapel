record ForwardingWrapper {
  type eltType;

  inline proc _value {
    return new C(eltType);
  }

  forwarding _value;
}


class C {
  type eltType;
  var lock$ : sync bool;
}

var wrapper : ForwardingWrapper(int);
