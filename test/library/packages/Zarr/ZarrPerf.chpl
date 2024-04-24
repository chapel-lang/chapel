use Zarr;
use Time;
use BlockDist;
use FileSystem;
use IO;
use Random;

config const scaling = "strong"; // testing strong or weak scaling

config const arrayGBs: real = 0.3; // default size of array in GBs (per locale)
config const chunkGBs: real = 0.1; // default size of chunks in GBs

config const compressionLevel: int(32) = 9; // default compression level

config const bloscThreads: int(32) = 4; // default number of threads for Blosc



var arraySize = 0;
if scaling.toLower() == "strong" then
  arraySize = (arrayGBs * Locales.size * 1000 ** 3):int;
else if scaling.toLower() == "weak" then
  arraySize = (arrayGBs * 1000 ** 3):int;
else
  writeln("Invalid scaling: %s. Valid options are 'strong' and 'weak'".format(scaling));

var chunkSize = chunkGBs * 1000 ** 3;

const numFloats = arraySize / 4;
const sideLength = (numFloats:real ** (1/3:real)):int;
const chunkLength = ((chunkSize / 4) ** (1/3:real)):int;

var ranges: 3*range(int);
for i in 0..<3 do
  ranges[i] = 0..<sideLength;
const undistD : domain(3) = ranges;
const Dist = new blockDist(boundingBox=undistD);
const D = Dist.createDomain(undistD);

var s: stopwatch;
var writeTime = -1.0;
var readTime = -1.0;

var A: [D] real(32);
fillRandom(A);

if exists("PerfStore") then rmTree("PerfStore");

s.restart();
writeZarrArray("PerfStore", A, (chunkLength,chunkLength,chunkLength), bloscThreads=bloscThreads, bloscLevel=compressionLevel);
writeTime = s.elapsed();

s.restart();
var B = readZarrArray("PerfStore", real(32), 3, bloscThreads=bloscThreads);
readTime = s.elapsed();


assert(A.domain == B.domain);
coforall loc in Locales do on loc {
  forall i in A.localSubdomain() do
    assert(A[i] == B[i], i, " ", A[i], " ", B[i]);
}

var writeThroughput: real(64) = (arraySize:real / writeTime) / 1000 ** 3;
var readThroughput: real(64) = (arraySize:real / readTime) / 1000 ** 3;
writeln("%s,%n,%n,%n,%n,%n,%7.4r,%7.4r".format(scaling, Locales.size, compressionLevel, bloscThreads, arrayGBs, chunkGBs, writeThroughput, readThroughput));
