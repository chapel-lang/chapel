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
  #define AUTO_JOINED_TEXT() ((AUTO_TEXT() " world"))
  #define OBJECT_TEXT AUTO_JOINED_TEXT()
  static char autoTextBuffer[] = "buffer";
  #define AUTO_BUFFER() autoTextBuffer
  static const char* autoConstTextBuffer = "const buffer";
  #define AUTO_CONST_BUFFER() autoConstTextBuffer
  #define AUTO_CAST_TEXT() ((char *)"cast")
  #define AUTO_CAST_CONST_TEXT() ((const char *)"cast const")
  #define AUTO_TEXT_OFFSET(n) (&"hello"[0] + (n))
  #define AUTO_TEXT_OFFSET_LEFT(n) ((n) + &"hello"[0])
  #define AUTO_TEXT_SUBSCRIPT(n) (&"hello"[(n)])
  #define AUTO_TEXT_DEREF() (&*"hello")
  #define AUTO_TEXT_COMMA() ((void)0, "hello")
  #define AUTO_TEXT_SELECT(n) ((n) ? "hello" : autoTextBuffer)
  #define AUTO_BUFFER_OFFSET(n) (autoTextBuffer + (n))
  #define AUTO_CAST_TEXT_OFFSET(n) (((char *)"cast") + (n))
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
assert(text.type == c_ptrConst(c_char));
assert(string.createCopyingBuffer(text) == "hello");
var joinedText = AUTO_JOINED_TEXT();
assert(joinedText.type == c_ptrConst(c_char));
assert(string.createCopyingBuffer(joinedText) == "hello world");
var objectText = OBJECT_TEXT;
assert(objectText.type == c_ptrConst(c_char));
assert(string.createCopyingBuffer(objectText) == "hello world");
var buffer = AUTO_BUFFER();
assert(buffer.type == c_ptr(c_char));
buffer[0] = 66:c_char;
assert(string.createCopyingBuffer(buffer) == "Buffer");
var constBuffer = AUTO_CONST_BUFFER();
assert(constBuffer.type == c_ptrConst(c_char));
assert(string.createCopyingBuffer(constBuffer) == "const buffer");
var castText = AUTO_CAST_TEXT();
assert(castText.type == c_ptr(c_char));
assert(string.createCopyingBuffer(castText) == "cast");
var castConstText = AUTO_CAST_CONST_TEXT();
assert(castConstText.type == c_ptrConst(c_char));
assert(string.createCopyingBuffer(castConstText) == "cast const");
var offsetText = AUTO_TEXT_OFFSET(1:c_int);
assert(offsetText.type == c_ptrConst(c_char));
assert(string.createCopyingBuffer(offsetText) == "ello");
assert(AUTO_TEXT_OFFSET_LEFT(1:c_int).type == c_ptrConst(c_char));
assert(AUTO_TEXT_SUBSCRIPT(1:c_int).type == c_ptrConst(c_char));
assert(AUTO_TEXT_DEREF().type == c_ptrConst(c_char));
assert(AUTO_TEXT_COMMA().type == c_ptrConst(c_char));
assert(AUTO_TEXT_SELECT(1:c_int).type == c_ptrConst(c_char));
assert(AUTO_BUFFER_OFFSET(1:c_int).type == c_ptr(c_char));
assert(AUTO_CAST_TEXT_OFFSET(1:c_int).type == c_ptr(c_char));
assert(AUTO_NEXT() == 1);
assert(nextFromAnotherFunction() == 2);
AUTO_BUMP(p, 3:c_int);
assert(i == 10 && AUTO_NEXT() == 6);
writeln("automatic macro types passed");
