use BlockDist;

config const b = 3;
config const n = 5;

const dataLocalDom = {1..n};
const dataDom = if CHPL_COMM == "none" then dataLocalDom
                else dataLocalDom dmapped Block(dataLocalDom);

var dataM: [dataDom] int = [i in 1..n] i % b + 1;
var dataB: [dataDom] int = 1..n;

type HistM = [1..b] int;
var histoM: HistM = 1;

type HistB = [1..n] uint(8);
var histoBA: HistB = _band_id(uint(8));
var histoBO: HistB = _bor_id(uint(8));
var histoBX: HistB = _bxor_id(uint(8));

proc showBits(name: string, histo: HistB) {
  writeln(name, " =");
  for h in histo do writef("  %08bu\n", h);
}

proc main {

  forall d in dataM with (* reduce histoM,
                          & reduce histoBA,
                          | reduce histoBO,
                          ^ reduce histoBX)
  {
    histoM[d] *= 2;
    histoBA[d]     &= ~(1<<d):uint(8); // make it 0 in that position
    histoBA[n+1-d] &= ~(1<<d):uint(8);
    histoBO[d]     |=  (1<<d):uint(8); // make it 1 in that position
    histoBO[n+1-d] |=  (1<<d):uint(8);
    histoBX[d]     |=  (1<<d):uint(8);
    histoBX[n+1-d] |=  (1<<d):uint(8);
  }

  writeln("histoM = ", histoM);
  showBits("histoBA", histoBA);
  showBits("histoBO", histoBO);
  showBits("histoBX", histoBX);

  histoBX = _bxor_id(uint(8));

  forall d in dataB with (^ reduce histoBX)
  {
    histoBX[d]     |=  (1<<d):uint(8);
    histoBX[n+1-d] |=  (1<<d):uint(8);
  }

  showBits("histoBX bis", histoBX);

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
