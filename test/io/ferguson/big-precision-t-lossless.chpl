use IO;

config const verbose = false;

{
  writeln("Testing default array");
  var A:[1..10] real;
  A = sqrt(1.0 / (1..10));
  A[5..10] = exp(-50.0 * (5..10));
  var B = A;

  if verbose then
    writef("%.20t\n", A);

  var testfile = openmem();
  {
    testfile.writer().writef("%.20t\n", A);
  }

  A = 0.0;

  {
    testfile.reader().readf("%t\n", A);
  }

  if verbose {
    writeln("Expected:");
    for x in B {
      writef("%r %.17er %xr\n", x, x, x);
    }
    writeln("Got:");
    for x in A {
      writef("%r %.17er %xr\n", x, x, x);
    }
  }

  writeln(B.equals(A));
}

{
  writeln("Testing default array with json");
  var A:[1..10] real;
  A = sqrt(1.0 / (1..10));
  A[5..10] = exp(-50.0 * (5..10));
  var B = A;

  if verbose then
    writef("%.20jt\n", A);

  var testfile = openmem();
  {
    testfile.writer().writef("%.20jt\n", A);
  }

  A = 0.0;

  {
    testfile.reader().readf("%jt\n", A);
  }

  if verbose {
    writeln("Expected:");
    for x in B {
      writef("%r %.17er %xr\n", x, x, x);
    }
    writeln("Got:");
    for x in A {
      writef("%r %.17er %xr\n", x, x, x);
    }
  }

  writeln(B.equals(A));
}
