//
// TODO: Wishlist writing this:
//
//  * assert()
//  * param for loop()
//  * halt()
//  * write 'print(x: ?t) where isTupleType(t)' using recursion
use Types;

extern {
  #include <stdint.h>
  #include <inttypes.h>
  #include <stdio.h>

  void printMyChar(int8_t x);
  void printMyInt64(int64_t x);
  void printMyReal64(double x);

  void printMyInt64(int64_t x) {
    printf("%" PRId64, x);
  }

  void printMyReal64(double x) {
    printf("%lf", x);
  }

  void printMyChar(int8_t x) {
    printf("%c", x);
  }
}

extern proc printMyChar(x: real(64));
extern proc printMyInt64(x: int(64));
extern proc printMyReal64(x: real(64));

proc doPrintSpace() do printMyChar(32);
proc doPrintNewline() do printMyChar(10);

private proc doPrintBool(x: bool) {
  if x {
    // Lol...
    printMyChar(116);
    printMyChar(114);
    printMyChar(117);
    printMyChar(101);
  } else {
    // Insert Picard 'facepalm' GIF...
    printMyChar(102);
    printMyChar(97);
    printMyChar(108);
    printMyChar(115);
    printMyChar(101);
  }
}

proc print(x: ?t) {
  if t == bool {
    doPrintBool(x);
  } else if t == int {
    printMyInt64(x);
  } else if t == real {
    printMyReal64(x);
  } else if isTupleType(t) {
    // TODO: Param loop this...
    // TODO: Move this to 'print'...
    if x.size >= 1 then print(x[0]);
    if x.size >= 2 { doPrintSpace(); print(x[1]); }
    if x.size >= 3 { doPrintSpace(); print(x[2]); }
    if x.size >= 4 { doPrintSpace(); print(x[3]); }
    if x.size >= 5 { doPrintSpace(); print(x[4]); }
    if x.size >= 6 { doPrintSpace(); print(x[5]); }
    if x.size >= 7 { doPrintSpace(); print(x[6]); }
    if x.size >= 8 { doPrintSpace(); print(x[7]); }
    if x.size >= 9 { doPrintSpace(); print(x[8]); }
  } else {
    // TODO: 'halt()'
  }
}

proc println(x) {
  print(x);
  doPrintNewline();
}
