//
// TODO: Wishlist writing this:
//
//  * assert()
//  * param for loop()
//  * halt()
//  * write 'print(x: ?t) where isTupleType(t)' using recursion
use Types;
use CTypes;

extern {
  #include <stdint.h>
  #include <inttypes.h>
  #include <stdio.h>
  #include <stdbool.h>

  void printMyChar(int8_t x);
  void printMyInt64(int64_t x);
  void printMyUint64(uint64_t x);
  void printMyReal64(double x);
  void printMyBool(bool x);
  void printStr(const char* str);

  void printMyInt64(int64_t x) {
    printf("%" PRId64, x);
  }

  void printMyUint64(uint64_t x) {
    printf("%" PRIu64, x);
  }

  void printMyReal64(double x) {
    printf("%lf", x);
  }

  void printMyChar(int8_t x) {
    printf("%c", x);
  }

  void printMyBool(bool x) {
    if (x) {
      printf("true");
    } else {
      printf("false");
    }
  }

  void printStr(const char* str) {
    printf("%s", str);
  }
}

extern proc printMyChar(x: real(64));
extern proc printMyInt64(x: int(64));
extern proc printMyUint64(x: uint(64));
extern proc printMyReal64(x: real(64));
extern proc printMyBool(x: bool);
extern proc printStr(str: c_ptrConst(c_char));

proc doPrintSpace() do printMyChar(32);
proc doPrintNewline() do printMyChar(10);

proc print(x: ?t) {
  if t == bool {
    printMyBool(x);
  } else if isIntType(t) {
    printMyInt64(x);
  } else if isUintType(t) {
    printMyUint64(x);
  } else if isRealType(t) {
    printMyReal64(x);
  } else if t == string {
    printStr(x.c_str());
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
  } else if isEnumType(t) {
    var temp : int = chpl__enumToOrder(x);
    printMyInt64(temp);
  } else {
    // TODO: 'halt()'
  }
}

proc println(x) {
  print(x);
  doPrintNewline();
}
