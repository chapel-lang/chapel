class myClass {
  var x = 10;
}

class myReduction : ReduceScanOp {
  type eltType;

  // Store minimum k items as vector in descending order.
  var v: myClass? = new myClass();

  proc identity {
    var v: myClass? = new myClass(); return v;
  }

  proc accumulateOntoState(ref v, value: eltType) {
    v!.x = 5;
  }

  proc accumulate(value: eltType) {
    accumulateOntoState(v, value);
  }

  proc accumulate(accumState: myClass) {
    accumulate(1);
  }
  
  proc accumulate(accumState: []) {
    for stateValue in accumState {
      accumulate(stateValue);
    }
  }

  proc combine(state: borrowed myReduction(eltType)) {
    accumulate(state.v);
  }

  proc generate() {
    return v!;
  }

  proc clone() {
    return new unmanaged myReduction(eltType=eltType);
  }
}

var a = myReduction reduce [1,2,3];
writeln(a.x);
