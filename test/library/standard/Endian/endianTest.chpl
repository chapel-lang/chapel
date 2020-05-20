use Endian;

config const debug = false;

// tests for 32-bit unsigned integer.
var test1: uint(32) = 10;
test(test1);
var test2: uint(32) = 32487;
test(test2);

// tests for 64-bit unsigned integer.
var test3: uint(64) = 10;
test(test3);
var test4: uint(64) = 9223372036854775800;
test(test4);

// tests for 32-bit real.
var test5: real(32) = 10.4;
test(test5);
var test6: real(32) = 104567.2;
test(test6);

// tests for 64-bit real.
var test7: real(64) = 10.4;
test(test7);
var test8: real(64) = 92233720368545800.6;
test(test8);

// tests for signed integer.
var test9: int = -100;
test(test9);
var test10: int = -32487;
test(test10);


proc test(x) {
  var a, b, c, d;

  // test for little endian
  a = hostToLittleEndian(x);
  b = littleEndianToHost(a);
  writeln(b == x);

  //test for big Endian
  c = hostToBigEndian(x);
  d = bigEndianToHost(c);
  writeln(d == x);

  if debug {
    writeln("original value");
    writef("%xn\n", x);
    writeln("little endian encoded");
    writef("%xn\n", a);
    writeln("big endian encoded");
    writef("%xn\n", c);
  }

}
