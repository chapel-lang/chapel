use CTypes;

extern {
  typedef struct {
    int x;
    double y;
  } macro_pair;
  static macro_pair makeMacroPair(int x) {
    macro_pair pair = {x, 2.5};
    return pair;
  }
  typedef unsigned int macro_count;
  static macro_count macroCount(void) { return 9; }
  static int autoMacroState = 0;
  #define AUTO_ADD(a, b) ((a) + (b))
  #define AUTO_TWICE(x) ((x) + (x))
  #define AUTO_SMALL(x) ((short)(x))
  #define AUTO_PAIR(x) makeMacroPair(x)
  #define AUTO_COUNT() macroCount()
  #define AUTO_ID(x) (x)
  #define AUTO_TEXT() "hello"
  #define AUTO_NEXT() (++autoMacroState)
  #define AUTO_CONST_KIND(p) _Generic((p), int *: 1, const int *: 2)
  #define AUTO_BUMP(p, x) do { *(p) += (x); autoMacroState += (x); } while (0)
}

var evaluations = 0;
proc nextArg(): c_int {
  evaluations += 1;
  return 3:c_int;
}

proc nextFromAnotherFunction(): c_int {
  return AUTO_NEXT();
}

var sum = AUTO_ADD(2:c_int, 3:c_int);
var mixed = AUTO_ADD(2:c_int, 0.5:c_double);
assert(sum.type == c_int && sum == 5);
assert(mixed.type == c_double && mixed == 2.5);
assert(AUTO_ADD(b=2:c_int, a=1:c_int) == 3);
assert(AUTO_TWICE(nextArg()) == 6 && evaluations == 1);
var small = AUTO_SMALL(8:c_int);
assert(small.type == c_short && small == 8);
var count = AUTO_COUNT();
assert(count.type == c_uint && count == 9);

var pair = AUTO_PAIR(17:c_int);
assert(pair.x.type == c_int && pair.x == 17 && pair.y == 2.5);
var pairCopy = AUTO_ID(pair);
assert(pairCopy.type == pair.type && pairCopy.x == 17);
var i: c_int = 7;
var p = AUTO_ID(c_ptrTo(i));
assert(p.type == c_ptr(c_int) && p.deref() == 7);
var constP = AUTO_ID(c_ptrToConst(i));
assert(constP.type == c_ptrConst(c_int) && constP.deref() == 7);
assert(AUTO_CONST_KIND(c_ptrTo(i)) == 1);
assert(AUTO_CONST_KIND(c_ptrToConst(i)) == 2);
var text = AUTO_TEXT();
assert(text.type == c_ptr(c_char));
assert(string.createCopyingBuffer(text) == "hello");
assert(AUTO_NEXT() == 1);
assert(nextFromAnotherFunction() == 2);
AUTO_BUMP(p, 3:c_int);
assert(i == 10 && AUTO_NEXT() == 6);
writeln("automatic macro types passed");
