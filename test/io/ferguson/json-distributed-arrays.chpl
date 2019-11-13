use BlockDist;
use CyclicDist;
use BlockCycDist;
use IO;

var expect = "[1, 2, 3, 4, 5]";

var expectfile = openmem();
{
  expectfile.writer().write(expect);
  // temporary writer flushed and closed at this curly
}

{
  writeln("Testing default array");
  var A:[1..5] int;
  A = 1..5;

  writef("%jt\n", A);
  var got = "%jt".format(A);
  assert(got == expect);

  A = 0;
  expectfile.reader().readf("%jt\n", A);
  writef("%jt\n", A);
  var got2 = "%jt".format(A);
  assert(got2 == expect);
}

{
  writeln("Testing block array");
  var A = newBlockArr({1..5}, int);
  A = 1..5;

  writef("%jt\n", A);
  var got = "%jt".format(A);
  assert(got == expect);
  
  A = 0;
  expectfile.reader().readf("%jt\n", A);
  writef("%jt\n", A);
  var got2 = "%jt".format(A);
  assert(got2 == expect);
}

{
  writeln("Testing cyclic array");
  var A = newCyclicArr({1..5}, int);
  A = 1..5;

  writef("%jt\n", A);
  var got = "%jt".format(A);
  assert(got == expect);
  
  A = 0;
  expectfile.reader().readf("%jt\n", A);
  writef("%jt\n", A);
  var got2 = "%jt".format(A);
  assert(got2 == expect);
}

{
  writeln("Testing block cyclic array");
  const Space = {1..5};
  var D = Space dmapped BlockCyclic(startIdx=Space.low,blocksize=2);
  var A:[D] int;
  A = 1..5;

  writef("%jt\n", A);
  var got = "%jt".format(A);
  assert(got == expect);
  
  A = 0;
  expectfile.reader().readf("%jt\n", A);
  writef("%jt\n", A);
  var got2 = "%jt".format(A);
  assert(got2 == expect);
}
