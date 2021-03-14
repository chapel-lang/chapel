class mink{

  type in_t;
  const k : int;
  var v: [1..k] in_t = in_t.max;
  
  proc accum(x: in_t)
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
  
  proc combine(s: mink(in_t))
  {
    for i in 1..k
      {
        accum(s.v[i]);
      }
  }
  
  proc gen()
  {
    return v;
  }
}
   
var A: [1..10] int;
forall i in 1..10 do
       A(i) = i;
var minimums: [1..10] int;
minimums = mink(int, 10) reduce A;

