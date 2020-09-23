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

// tests for signed integer.
var test5: int = -100;
test(test5);
var test6: int = -32487;
test(test6);

// tests for 16-bit unsigned integer.
var test7: uint(16) = 10;
test(test7);
var test8: uint(16) = 65500;
test(test8);

// tests for 8-bit unsigned integer.
var test9: uint(8) = 10;
test(test9);
var test10: uint(8) = 255;
test(test10);

/*
   The conditional statements are used to make the `.good` file
   pass for both little and big endian systems. If the host is little
   endian `hostToLittleEndian(x)` will return the value unchanged, so
   will `hostToBigEndian(x)` in case of a big endian host, so the order
   of the function calls is changed in the else statement to match the
   `.good` file.
*/
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
