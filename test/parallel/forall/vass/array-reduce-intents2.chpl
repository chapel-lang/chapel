use BlockDist;

config const b = 3;
config const n = 5;

const dataLocalDom = {1..n};
const dataDom = dataLocalDom dmapped Block(dataLocalDom); // problem
//const dataDom = dataLocalDom; // this works

var dataM: [dataDom] int = [i in 1..n] i % b + 1;
var dataB: [dataDom] int = 1..n;

type HistM = [1..b] int;
var histoM: HistM = 1;
var histoP: HistM = 0;

proc main {

  forall d in dataM with (* reduce histoM,
                          PlusReduceOp reduce histoP)
  {
    histoM[d] *= 2;
    histoP[d] += 2;
  }

  // cute: histoM and histoP are the same
  writeln("histoM = ", histoM);
  writeln("histoP = ", histoP);
}

class PlusReduceOp: ReduceScanOp {
  type eltType;
  var  value: eltType;
  proc identity         return 0: eltType;
  proc accumulate(elm)  { value = value + elm; }
  proc combine(other)   { value = value + other.value; }
  proc generate()       return value;
  proc clone()          return new unmanaged PlusReduceOp(eltType=eltType);
}
