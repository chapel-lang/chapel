use Schedules;

config const lo = 1;
config const hi = 100;

config const numBlocks = 3;
config const blockSize = 15;

var D = [lo..hi];

/*
def BlockSchedule(D: ?DT, numBlocks): DT {
  const lo = D(1)._low;
  const hi = D(1)._high;
  const numelems = hi - lo + 1;
  forall i in 0..numBlocks-1 {
    const blo = lo + (i*numelems) / numBlocks;
    const bhi = lo + (((i+1)*numelems) / numBlocks) - 1;
    yield [blo..bhi];
  }
}


def FixedBlockSchedule(D: ?DT, blockSize): DT {
  const lo = D(1)._low;
  const hi = D(1)._high;
  const numelems = hi - lo + 1;
  forall i in 0..ceil(numelems:real/blockSize):int - 1 {
    const blo = lo + i*blockSize;
    const bhi = min(lo + (i+1)*blockSize-1, hi);
    yield [blo..bhi];
  }
}
*/


for b in BlockSchedule(D, numBlocks) {
  writeln("b is: ", b);
}
writeln();

for b in FixedBlockSchedule(D, blockSize) {
  writeln("b is: ", b);
}
