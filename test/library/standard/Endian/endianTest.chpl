use Endian;

// tests for 32-bit unsigned integer.
var test1: uint(32) = 10;
test(test1);
var test2: uint(32) = 2147483600;
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

// tests for 16-bit unsigned integer.
var test11: uint(16) = 10;
test(test11);
var test12: uint(16) = 65500;
test(test12);

proc test(x) {
  var a, b, c, d;

  if isLittleEndian {
    // test for little endian
    a = hostToLittleEndian(x);
    writef("%xn\n", a);
    b = littleEndianToHost(a);
    writef("%xn\n", b);
    writeln(b == x);

    //test for big Endian
    c = hostToBigEndian(x);
    writef("%xn\n", c);
    d = bigEndianToHost(c);
    writef("%xn\n", d);
    writeln(d == x);
  } else {
    //test for big Endian
    a = hostToBigEndian(x);
    writef("%xn\n", a);
    b = bigEndianToHost(a);
    writef("%xn\n", b);
    writeln(b == x);

    // test for little endian
    c = hostToLittleEndian(x);
    writef("%xn\n", c);
    d = littleEndianToHost(c);
    writef("%xn\n", d);
    writeln(d == x);
  }

}
