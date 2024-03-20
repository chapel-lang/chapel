use Zarr;
use Time;
use BlockDist;
use FileSystem;
use IO;
proc throughputTest(type dtype, param dimCount: int, arrayShape: dimCount*int, chunkShape: dimCount*int, bloscThreads: int(32)) {
  var ranges: dimCount*range(int);
  for i in 0..<dimCount do
    ranges[i] = 0..<arrayShape[i];
  const undistD : domain(dimCount) = ranges;
  const Dist = new blockDist(boundingBox=undistD);
  const D = Dist.createDomain(undistD);
  var A: [D] dtype;

  coforall loc in Locales do on loc {
    forall i in A.localSubdomain() do
      A[i] = if isTuple(i) then i[0]:dtype else i:dtype;
  }

  if exists("PerfStore") then rmTree("PerfStore");
  var s: stopwatch;
  s.start();
  writeZarrArray("PerfStore", A, chunkShape, bloscThreads=bloscThreads);
  const writeTime = s.elapsed();

  s.restart();
  var B = readZarrArray("PerfStore", dtype, dimCount);
  const readTime = s.elapsed();

  assert(A.domain == B.domain);
  coforall loc in Locales do on loc {
    forall i in A.localSubdomain() do
      assert(A[i] == B[i]);
  }

  const arrayBytes = A.size:real * numBits(dtype) / 8;
  const arrayGBs = arrayBytes:real / (1000 ** 3);

  const chunkBytes = (* reduce chunkShape) * numBits(dtype):real / 8;
  const chunkMBs = chunkBytes:real / (1000 ** 2);
  writeln("%?GB %?D array of %?, %?MB chunks. %n locales, %n bloscThreads".format(arrayGBs, dimCount, dtype:string, chunkMBs, Locales.size, bloscThreads));
  writeln("%n GB/s writing".format(arrayGBs / writeTime));
  writeln("%n GB/s reading".format(arrayGBs / readTime));
}


throughputTest(real(32), 1, (1000**3,), (1000**2,),1);
throughputTest(real(32), 1, (1000**3,), (1000**2,),2);
throughputTest(real(32), 1, (1000**3,), (1000**2,),4);
throughputTest(real(32), 2, (30000,30000), (1000,1000),1);
throughputTest(real(32), 2, (30000,30000), (1000,1000),2);
throughputTest(real(32), 2, (30000,30000), (1000,1000),4);
throughputTest(real(32), 3, (1000,1000,1000), (100,100,100),1);
throughputTest(real(32), 3, (1000,1000,1000), (100,100,100),2);
throughputTest(real(32), 3, (1000,1000,1000), (100,100,100),4);
throughputTest(int(32), 1, (1000**3,), (1000**2,),1);
throughputTest(int(32), 1, (1000**3,), (1000**2,),2);
throughputTest(int(32), 1, (1000**3,), (1000**2,),4);
throughputTest(int(32), 2, (30000,30000), (1000,1000),1);
throughputTest(int(32), 2, (30000,30000), (1000,1000),2);
throughputTest(int(32), 2, (30000,30000), (1000,1000),4);
throughputTest(int(32), 3, (1000,1000,1000), (100,100,100),1);
throughputTest(int(32), 3, (1000,1000,1000), (100,100,100),2);
throughputTest(int(32), 3, (1000,1000,1000), (100,100,100),4);
