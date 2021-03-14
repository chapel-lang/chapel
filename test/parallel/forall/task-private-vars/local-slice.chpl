use BlockDist;

config const n = 36;
config const dptpl = 3;

const D = {1..n};
const BD = D dmapped Block(D,
                           dataParTasksPerLocale = dptpl);
var A: [BD] int = 1..n;

writeln("before: ", A);

forall idx in BD with (var localCount = (here.id * 100000),
                       ref localSlice = A.localSlice(A.localSubdomain()))
{
  local {
    localCount += 1000;
    localSlice[idx] += localCount;
  }
}

writeln("after: ", A);
