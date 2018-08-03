var XXX = 7, YYYmax = 3, YYYmin = -1, ZZZ = 47, WWW = 8: int;

proc main {

  coforall idx in 1..10 with
     (+ reduce XXX, max reduce YYYmax, min reduce YYYmin,
      in ZZZ, MyReduceOp reduce WWW)
  {
    XXX = 500;
    YYYmax = idx;
    YYYmin = idx;
    writeln(ZZZ);
    ZZZ = 333;
    WWW += 300;
  }

  writeln((XXX, YYYmax, YYYmin, ZZZ, WWW));

}

class MyReduceOp: ReduceScanOp {
  type eltType;
  var  value: eltType;
  proc identity         return 0: eltType;
  proc accumulate(elm)  { value = value + elm; }
  proc combine(other)   { value = value + other.value; }
  proc generate()       return value;
  proc clone()          return new unmanaged MyReduceOp(eltType=eltType);
}
