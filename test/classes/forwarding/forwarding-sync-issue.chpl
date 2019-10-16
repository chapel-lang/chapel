// This test came from issue #8449
// At one point it failed to compile.

var global:unmanaged object?;

record ForwardingWrapper {
  type eltType;

  inline proc _value {
    if global != nil then
      delete global;
    var ret = new unmanaged C(eltType);
    global = ret;
    return ret;
  }

  forwarding _value;
}


class C {
  type eltType;
  var lock$ : sync bool;
}

var wrapper : ForwardingWrapper(int);

writeln(wrapper.lock$.isFull);

delete global;
