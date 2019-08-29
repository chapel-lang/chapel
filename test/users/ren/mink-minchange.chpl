class mink : ReduceScanOp {

  type eltType;
  const k : int = 10;
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
                var tmp = v[i];
                v[i] = v[i-1];
                v[i-1] = tmp;
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
    return v;
  }
  
  proc clone() return new unmanaged mink(eltType=eltType);
}
   
var A: [1..10] int;
forall i in 1..10 do
       A(i) = i;
var minimums: [1..10] int;
minimums = mink reduce A;

writeln("minimums is: ", minimums);
