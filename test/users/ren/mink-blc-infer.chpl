config const n = 10,
             numMins = 5;

class mink : ReduceScanOp {

  type eltType;
  const k : int = numMins;
  var v: [1..k] eltType = max(eltType);
  
  proc identity return [1..k] max(eltType);
  
  proc accumulate(x: eltType)
  {
    accumulateOntoState(v, x);
  }
  
  proc accumulateOntoState(state, x: eltType)
  {
    if (x < v[1])
      {
        v[1] = x;
        for i in 2..k
          {
            if (v[i-1] < v[i])
              {
                v[i] <=> v[i-1];
              }
          }
      }
  }
  
  proc accumulate(state: [])
  {
    for i in 1..k
      {
        accumulate(state[i]);
      }
  }
  
  proc combine(s: borrowed mink(eltType)) {
    accumulate(s.v);
  }

  proc generate()
  {
    writeln("returning: ", v);
    return v;
  }
  
  proc clone() return new unmanaged mink(eltType=eltType);
}

var A: [1..n] int = 1..n;
var minimums = mink reduce A;

writeln("minimums is: ", minimums);
