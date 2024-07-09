use IO;

config const nWorkers = 4;

var f = open("readConcurrent.dat", ioMode.r);

const nBytes = f.size,
      nPerLoc = nBytes/ nWorkers;

var a: [0..<nBytes] uint(8);

coforall w in 0..<nWorkers {
  const myRegion = (w*nPerLoc)..<((w+1) * nPerLoc),
        fr = f.reader(region=myRegion, locking=false);

  fr.readBinary(a[myRegion]);
}

writeln(a);
