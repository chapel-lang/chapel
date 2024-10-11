use Zarr;
use IO;
use FileSystem;
use Random;

proc test3D() {
  const fullD: domain(3) = {0..10, 0..10, 0..10};
  const partialD: domain(3) = {0..5, 0..5, 0..5};

  var ones: [fullD] real(32) = 1;
  var twos: [fullD] real(32) = 2;

  if (isDir("ones")) then rmTree("ones");
  if (isDir("twos")) then rmTree("twos");
  writeZarrArray("ones", ones, (3,3,3));
  writeZarrArray("twos", twos, (3,3,3));

  var inputData = readZarrArray("ones", real(32), 3);
  inputData[partialD] = readZarrArrayPartial("twos", real(32), 3, partialD);

  var expectedData: [fullD] real(32) = 1.0;
  expectedData[partialD] = 2.0;

  assert(inputData.domain == expectedData.domain, "Domain mismatch: %? %?".format(inputData.domain, expectedData.domain));
  forall i in inputData.domain do 
    assert(inputData[i] == expectedData[i], "Mismatch for 3D real data on indices: %?.\nWritten: %?\nRead: %?".format(i, inputData[i], expectedData[i]));
  rmTree("ones");
  rmTree("twos");
}

proc testOutOfBounds() {
  const fullD: domain(2) = {0..10, 0..10};
  const partialD: domain(2) = {10..10, 10..12};

  var ones: [fullD] real(32) = 1;
  var twos: [fullD] real(32) = 2;

  if (isDir("ones")) then rmTree("ones");
  if (isDir("twos")) then rmTree("twos");
  writeZarrArray("ones", ones, (3,3));
  writeZarrArray("twos", twos, (3,3));

  var inputData = readZarrArray("ones", real(32), 2);
  try {
    var outOfBoundsEdge = readZarrArrayPartial("twos", real(32), 2, partialD);
  } catch e {
    assert(e.message() == "Partial domain is out of bounds of the array domain.", "Unexpected error message: %s".format(e.message()));
    return;
  }
  writeln("Expected error for out of bounds partial domain.");
}
proc main() {
  
  test3D();
  testOutOfBounds();
  writeln("Pass");
}
