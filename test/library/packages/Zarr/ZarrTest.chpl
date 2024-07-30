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

proc smallTest(type dtype) {
  {
    const N1 = 100;
    const D1: domain(1) dmapped new blockDist({0..<N1}) = {0..<N1};
    var A1: [D1] dtype;
    for i in D1 do A1[i] = (i + 3):dtype;
    if (isDir("Test1D")) then rmTree("Test1D");
    writeZarrArray("Test1D", A1, (7,));

    var B1 = readZarrArray("Test1D", dtype, 1);
    
    assert(A1.domain == B1.domain, "Domain mismatch: %? %?".format(A1.domain, B1.domain));
    forall i in A1.domain do 
      assert(A1[i] == B1[i], "Mismatch for 1D real data on indices: %?.\nWritten: %?\nRead: %?".format(i, A1[i], B1[i]));
    rmTree("Test1D");
  }

  {
    const N2 = 100;
    const D2: domain(2) dmapped new blockDist({0..<N2,0..<N2}) = {0..<N2,0..<N2};
    var A2: [D2] dtype;
    for (i,j) in D2 do A2[i,j] = (i:real(32) / (j+1)) : dtype;
    if (exists("Test2D")) then rmTree("Test2D");
    writeZarrArray("Test2D", A2, (7,18));

    var B2 = readZarrArray("Test2D", dtype, 2);
    
    assert(A2.domain == B2.domain, "Domain mismatch: %? %?".format(A2.domain, B2.domain));
    forall i in A2.domain do 
      assert(A2[i] == B2[i], "Mismatch for 2D real data on indices: %?.\nWritten: %?\nRead: %?".format(i, A2[i], B2[i]));
    rmTree("Test2D");
  }

  {
    const N3 = 30;
    const D3: domain(3) dmapped new blockDist({0..<N3,0..<N3,0..<N3}) = {0..<N3,0..<N3,0..<N3};
    var A3: [D3] dtype;
    for (i,j,k) in D3 do A3[i,j,k] = (i:real(32) / ((j+1) + (k+1))) : dtype;
    if (exists("Test3D")) then rmTree("Test3D");
    writeZarrArray("Test3D", A3, (7,18,22));

    var B3 = readZarrArray("Test3D", dtype, 3);
    
    assert(A3.domain == B3.domain, "Domain mismatch: %? %?".format(A3.domain, B3.domain));
    forall i in A3.domain do 
      assert(A3[i] == B3[i], "Mismatch for 3D real data on indices: %?.\nWritten: %?\nRead: %?".format(i, A3[i], B3[i]));
    rmTree("Test3D");
  }
}

proc reindexTest(type dtype) {
  const D = {1..20,14..<72,-10..10};
  var A: [D] dtype;
  forall (i,j,k) in D do A[i,j,k] = (i + j*i + k*j):dtype;
  if exists("ReindexStore") then rmTree("ReindexStore");
  writeZarrArray("ReindexStore", A, (5,8,12));

  var B = readZarrArray("ReindexStore", dtype, 3);
  ref viewB = B.reindex(D);
  assert(A.domain == viewB.domain, "Domain mismatch for array with offset dimensions");
  forall i in D do
    assert(A[i] == viewB[i], "Mismatch on indices %?. Written: %?\n. Read: %?\n.".format(i, A[i], viewB[i]));
  
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
    const fullDomain = {0..<1, 0..<100, 0..<1082, 0..<361};
    const chunkSize = (1,100,136,23);
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
  const typeTuple = (0:int(64), 0:real(32));
  for param i in 0..<typeTuple.size {
    type dtype = typeTuple[i].type;
    writeln("Testing ", dtype:string);
    smallTest(dtype);
    reindexTest(dtype);
    localIOTest(dtype);
    updateChunkTest(dtype);
  }
  writeln("Pass");
}
