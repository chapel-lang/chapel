use Zarr;
use IO;
use FileSystem;
use Random;


proc main() {
  const fullD: domain(3) = {0..10, 0..10, 0..10};
  const partialD1: domain(3) = {0..5, 0..5, 0..5};

  var ones: [fullD] real(32) = 1;
  var twos: [fullD] real(32) = 2;

  if (isDir("ones")) then rmTree("ones");
  if (isDir("twos")) then rmTree("twos");
  writeZarrArray("ones", ones, (3,3,3));
  writeZarrArray("twos", twos, (3,3,3));

  var inputData = readZarrArray("ones", real(32), 3);
  inputData[partialD1] = readZarrArrayPartial("twos", real(32), 3, partialD1);

  var expectedData: [fullD] real(32) = 1.0;
  expectedData[partialD1] = 2.0;

  assert(inputData.domain == expectedData.domain, "Domain mismatch: %? %?".format(inputData.domain, expectedData.domain));
  forall i in inputData.domain do 
    assert(inputData[i] == expectedData[i], "Mismatch for 3D real data on indices: %?.\nWritten: %?\nRead: %?".format(i, inputData[i], expectedData[i]));
  rmTree("ones");
  rmTree("twos");

  writeln("Pass");
}
