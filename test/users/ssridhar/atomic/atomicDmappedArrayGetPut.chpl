use BlockDist, Time;

type indexType = int(64),
  elemType = int(64);

config const n: indexType = 4;

const m = 2**n;

config const printArrays = false,
  printStats = true;

const TableDist = new dmap(new Block(boundingBox=[0..m-1]));

const TableSpace: domain(1, indexType) dmapped TableDist = [0..m-1];

var T: [TableSpace] elemType;

def main() {
  var i: indexType;

  [i in TableSpace] T(i) = i;

  if (printArrays) then writeln("Before updates T = \n", T);

  const startTime = getCurrentTime();

  for i in 0..m-1 {
    atomic {
      T(i) += 1;
      T((i+1) % m) += 1;
    }
  }

  const endTime = getCurrentTime() - startTime;

  if (printArrays) then writeln("After updates T = \n", T);

  const numErrors = + reduce [i in TableSpace] (T(i) != (i + 2));
  writeln("Validation: ", if numErrors then "FAILURE" else "SUCCESS");
  if (printStats) then writeln("Time = ", endTime);
}
