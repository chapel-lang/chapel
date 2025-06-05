use Zarr;
use IO;
use FileSystem;
use BlockDist;
use Random;

proc localTest(compressor: string) {
  const N = 20;
  const D: domain(2) = {0..<N, 0..<2*N};
  var A: [D] real(32);
  fillRandom(A);

  if (exists("TestLocal")) then rmTree("TestLocal");
  writeZarrArrayLocal("TestLocal", A, (7,18), compressor=compressor);

  var B = readZarrArrayLocal("TestLocal", real(32), 2);
  assert(A.domain == B.domain, "Domain mismatch: %? %?".format(A.domain, B.domain));
  forall i in A.domain do 
    assert(A[i] == B[i], "Mismatch for real data on indices: %?.\nWritten: %?\nRead: %?".format(i, A[i], B[i]));
  rmTree("TestLocal");
}

proc distributedTest(compressor: string) {
  const N = 20;
  const D: domain(2) dmapped new blockDist({0..N, 0..<2*N}) = {0..<N, 0..<2*N};
  var A: [D] real(32);
  fillRandom(A);

  if (exists("TestDistributed")) then rmTree("TestDistributed");
  writeZarrArray("TestDistributed", A, (7,18), compressor=compressor);

  var B = readZarrArray("TestDistributed", real(32), 2);
  assert(A.domain == B.domain, "Domain mismatch: %? %?".format(A.domain, B.domain));
  forall i in A.domain do 
    assert(A[i] == B[i], "Mismatch for real data on indices: %?.\nWritten: %?\nRead: %?".format(i, A[i], B[i]));
  rmTree("TestDistributed");
}

proc testUnsupportedCompressor() {
  var A: [1..10] real(32);
  try {
    writeZarrArrayLocal("TestUnsupportedCompressor", A, (7,), compressor="unsupported");
    assert(false, "Expected an error for unsupported compressor");
  } catch e {
    assert(e.message() == "Unsupported compressor: unsupported. Supported compressors are: blosclz, lz4, lz4hc, zlib, and zstd.", e.message());
  }
  try {
    writeZarrArray("TestUnsupportedCompressor", A, (7,), compressor="unsupported");
    assert(false, "Expected an error for unsupported compressor");
  } catch e {
    assert(e.message() == "Unsupported compressor: unsupported. Supported compressors are: blosclz, lz4, lz4hc, zlib, and zstd.", e.message());
  }
  
}

proc testNoCompressorSpecified() {
  const configJson = '''
    {
      "zarr_format": 2,
      "chunks": [5,8,12],
      "shape": [10, 20, 30],
      "dtype": "<f4"
    }
    ''';
  
  var A: [0..<10,0..<20,0..<30] real(32);
  writeZarrArrayLocal("TestNoCompressorSpecified", A, (5,8,12));
  var configFile = openWriter("TestNoCompressorSpecified/.zarray");
  configFile.write(configJson);
  configFile.close();

  var B = readZarrArrayLocal("TestNoCompressorSpecified", real(32), 3);
  forall i in A.domain do 
    assert(A[i] == B[i], "Mismatch for real data on indices: %?.\nWritten: %?\nRead: %?".format(i, A[i], B[i]));
}


proc main() {
  var compressors = ["blosclz", "lz4", "lz4hc", "zlib", "zstd"];
  for compressor in compressors {
    writeln("Testing ", compressor);
    localTest(compressor);
    distributedTest(compressor);
  }
  testUnsupportedCompressor();
  testNoCompressorSpecified();
  writeln("Pass");
}
