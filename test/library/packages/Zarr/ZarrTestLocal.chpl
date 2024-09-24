use Zarr;
use IO;
use FileSystem;
use BlockDist;
use Random;

proc testGetLocalChunks() {
  var D: domain(1) dmapped new blockDist({0..20}) = {0..20};
  var local1 = {0..10};
  var local2 = {11..20};
  var chunks1 = getLocalChunks(D, local1, (7,));
  var chunks2 = getLocalChunks(D, local2, (7,));

  assert(chunks1 == {0..1}, "failed for local1 chunks: %?".format(chunks1));
  assert(chunks2 == {1..2}, "failed for local2 chunks: %?".format(chunks2));

  var D2: domain(2) = {0..20,0..20};
  var local3 = {0..20,0..20};
  var chunks3 = getLocalChunks(D2, local3, (7,7));
  assert(chunks3 == {0..2,0..2}, "failed for local3 chunks: %?".format(chunks3));
  var local4 = {0..0,0..0};
  var chunks4 = getLocalChunks(D2, local4, (7,7));
  assert(chunks4 == {0..0,0..0}, "failed for local4 chunks: %?".format(chunks4));
}

proc testUndistributedArray() {
  const N = 100;
  const D: domain(1) = {0..<N};
  var A: [D] int;
  for i in D do A[i] = i;
  if (exists("Test1D")) then rmTree("Test1D");
  writeZarrArray("Test1D", A, (7,));

  var B = readZarrArray("Test1D", int, 1);
  
  assert(A.domain == B.domain, "Domain mismatch: %? %?".format(A.domain, B.domain));
  forall i in A.domain do 
    assert(A[i] == B[i], "Mismatch for 1D int data on indices: %?.\nWritten: %?\nRead: %?".format(i, A[i], B[i]));
  rmTree("Test1D");
}

proc localIOTest(type dtype) {
  // Test having each locale read and write a different store
  const N = 100;
  coforall loc in Locales do on loc {
    const storeName = "LocalIOStore_%?".format(loc.id);
    const D = {0..<N};
    var A: [D] dtype;
    fillRandom(A);
    if exists(storeName) then rmTree(storeName);
    writeZarrArrayLocal(storeName, A, (7,));
    var B = readZarrArrayLocal(storeName, dtype, 1);
    forall i in D do
      assert(A[i] == B[i], "Mismatch on indices %?. Written: %?.\n Read: %?.\nFailure on locale %?\n".format(i, A[i], B[i], loc.id));
  }
  
  // 2 dimensional stores with different low bounds
  coforall loc in Locales do on loc {
    const storeName = "LocalIOStore_%?".format(loc.id);
    const D = {0..<N,1..N};
    var A: [D] dtype;
    fillRandom(A);
    if exists(storeName) then rmTree(storeName);
    writeZarrArrayLocal(storeName, A, (7,22));
    var B = readZarrArrayLocal(storeName, dtype, 2);
    ref viewB = B.reindex(D);
    forall i in D do
      assert(A[i] == viewB[i], "Mismatch on indices %?. Written: %?.\n Read: %?.\nFailure on locale %?\n".format(i, A[i], viewB[i], loc.id));
  }

  // 4 dimensional test from Scott's temperature dataset
  coforall loc in Locales do on loc {
    const storeName = "LocalIOStore_%?".format(loc.id);
    const fullDomain = {0..<1, 0..<10, 0..<108, 0..<36};
    const chunkSize = (1,10,13,2);
    var A: [fullDomain] dtype;
    fillRandom(A);
    if exists(storeName) then rmTree(storeName);
    writeZarrArrayLocal(storeName, A, chunkSize);
    var B = readZarrArrayLocal(storeName, dtype, 4);
    ref viewB = B.reindex(fullDomain);
    forall i in fullDomain do
      assert(A[i] == viewB[i], "Mismatch on indices %?. Written: %?.\n Read: %?.\nFailure on locale %?\n".format(i, A[i], viewB[i], loc.id));
  }
}

proc updateChunkTest(type dtype) {
  const N = 100;
  const N1 = 100;
  const D1: domain(1) dmapped new blockDist({0..<N1}) = {0..<N1};
  var A1: [D1] dtype;
  for i in D1 do A1[i] = (i + 3):dtype;
  if isDir("Test1D") then rmTree("Test1D");
  writeZarrArray("Test1D", A1, (7,));

  A1[10] = -1;
  updateZarrChunk("Test1D", A1, 1);

  var B1 = readZarrArray("Test1D", dtype, 1);

  assert(B1[10] == -1, "Failed to update chunk in 1D array");

  rmTree("Test1D");

  const N2 = 100;
  const D2: domain(2) dmapped new blockDist({0..<N2,0..<N2}) = {0..<N2,0..<N2};
  var A2: [D2] dtype;
  fillRandom(A2);
  if exists("Test2D") then rmTree("Test2D");
  writeZarrArray("Test2D", A2, (7,18));

  A2[10,10] *= 3;
  updateZarrChunk("Test2D", A2, (1,0));

  var B2 = readZarrArray("Test2D", dtype, 2);
  assert(B2[10,10] == A2[10,10], "Failed to update chunk in 2D array");
  
}

proc main() {
  testGetLocalChunks();
  testUndistributedArray();
  const typeTuple = (0:int(64), 0:int(32), 0:real(64), 0:real(32));
  for param i in 0..<typeTuple.size {
    type dtype = typeTuple[i].type;
    writeln("Testing ", dtype:string);
    localIOTest(dtype);
    updateChunkTest(dtype);
  }
  writeln("Pass");
}
