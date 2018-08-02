config const n = 10,
             numMins = 5;

class mink : ReduceScanOp {

  type eltType;
  const k : int = numMins;
  var v: [1..k] eltType = max(eltType);
  
  proc accumulate(x: eltType)
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
  
  proc combine(s: borrowed mink(eltType))
  {
    for i in 1..k
      {
        accumulate(s.v[i]);
      }
  }
  
  proc generate()
  {
    writeln("returning: ", v);
    return v;
  }
}

var A: [1..n] int = 1..n;
var minimums = mink reduce A;

writeln("minimums is: ", minimums);
