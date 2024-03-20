use Zarr;
use Time;
use BlockDist;
use FileSystem;
use IO;
use Random;

config const scaling = "strong"; // testing strong or weak scaling

config const gbs = 10; // default size of array in GBs

var arraySize = 0;

if scaling.toLower() == "strong" then
  arraySize = gbs * Locales.size * 1000 ** 3;
else if scaling.toLower() == "weak" then
  arraySize = gbs * 1000 ** 3;
else
  writeln("Invalid scaling: %s. Valid options are 'strong' and 'weak'".format(scaling));

const numFloats = arraySize / 4;
const sideLength = (numFloats:real ** (1/3:real)):int;

var ranges: 3*range(int);
for i in 0..<3 do
  ranges[i] = 0..<sideLength;
const undistD : domain(3) = ranges;
const Dist = new blockDist(boundingBox=undistD);
const D = Dist.createDomain(undistD);

var A: [D] real(32);
fillRandom(A);

if exists("PerfStore") then rmTree("PerfStore");
var s: stopwatch;
s.restart();
writeZarrArray("PerfStore", A, (512,512,512), bloscThreads=4);
const writeTime = s.elapsed();


s.restart();
var B = readZarrArray("PerfStore", real(32), 3, bloscThreads=4);
const readTime = s.elapsed();

assert(A.domain == B.domain);
coforall loc in Locales do on loc {
  forall i in A.localSubdomain() do
    assert(A[i] == B[i], i, " ", A[i], " ", B[i]);
}


writeln("Scaling: %s".format(scaling.toLower()));
writeln("Num Locales: %n".format(Locales.size));
var writeThroughput: real(64) = (arraySize:real / writeTime) / 1000 ** 3;
var readThoughput: real(64) = (arraySize:real / readTime) / 1000 ** 3;
writeln("Write Throughput: %7.2r GB/s".format(writeThroughput));
writeln("Read Throughput: %7.2r GB/s".format(readThoughput));
