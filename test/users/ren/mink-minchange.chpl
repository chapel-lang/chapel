class mink : ReduceScanOp {

  type eltType;
  const k : int = 10;
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
                var tmp = v[i];
                v[i] = v[i-1];
                v[i-1] = tmp;
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
    return v;
  }
}
   
var A: [1..10] int;
forall i in 1..10 do
       A(i) = i;
var minimums: [1..10] int;
minimums = mink reduce A;

writeln("minimums is: ", minimums);
