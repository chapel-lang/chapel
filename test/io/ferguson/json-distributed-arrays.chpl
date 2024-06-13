use BlockDist;
use CyclicDist;
use BlockCycDist;
use IO;
use JSON;

var jsonOut = stdout.withSerializer(jsonSerializer);

var expect = "[1, 2, 3, 4, 5]";

var expectfile = openMemFile();
{
  expectfile.writer(locking=false).write(expect);
  // temporary writer flushed and closed at this curly
}

{
  writeln("Testing default array");
  var A:[1..5] int;
  A = 1..5;

  jsonOut.writef("%?\n", A);
  var got = jsonify(A);
  assert(got == expect);

  A = 0;
  expectfile.reader(deserializer = new jsonDeserializer(), locking=false).readf("%?\n", A);
  jsonOut.writef("%?\n", A);
  var got2 = jsonify(A);
  assert(got2 == expect);
}

{
  writeln("Testing block array");
  var A = blockDist.createArray({1..5}, int);
  A = 1..5;

  jsonOut.writef("%?\n", A);
  var got = jsonify(A);
  assert(got == expect);

  A = 0;
  expectfile.reader(deserializer = new jsonDeserializer(), locking=false).readf("%?\n", A);
  jsonOut.writef("%?\n", A);
  var got2 = jsonify(A);
  assert(got2 == expect);
}

{
  writeln("Testing cyclic array");
  var A = cyclicDist.createArray({1..5}, int);
  A = 1..5;

  jsonOut.writef("%?\n", A);
  var got = jsonify(A);
  assert(got == expect);

  A = 0;
  expectfile.reader(deserializer = new jsonDeserializer(), locking=false).readf("%?\n", A);
  jsonOut.writef("%?\n", A);
  var got2 = jsonify(A);
  assert(got2 == expect);
}

{
  writeln("Testing block cyclic array");
  const Space = {1..5};
  var D = Space dmapped new blockCycDist(startIdx=Space.low,blocksize=2);
  var A:[D] int;
  A = 1..5;

  jsonOut.writef("%?\n", A);
  var got = jsonify(A);
  assert(got == expect);

  A = 0;
  expectfile.reader(deserializer = new jsonDeserializer(), locking=false).readf("%?\n", A);
  jsonOut.writef("%?\n", A);
  var got2 = jsonify(A);
  assert(got2 == expect);
}

proc jsonify(A): string throws {
  var f = openMemFile();
  f.writer(serializer = new jsonSerializer(), locking=false).write(A);
  return f.reader(locking=false).readAll(string);
}
