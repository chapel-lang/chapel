use CTypes;

extern {
  static int macroState = 41;
  #define MACRO_PTR (&macroState)
  #define MACRO_NESTED ((unsigned long)(1 + 2 * 3))
  #define MACRO_RUNTIME (++macroState)
  #define MACRO_SIZE sizeof(macroState)
  #define MACRO_SELECT (macroState == 41 ? 3 : 4)
  #define MACRO_TEXT (&"hello"[1])
  static const int macroConstState = 17;
  static int* const macroConstPtr = &macroState;
  typedef const int macro_const_int;
  static macro_const_int* macroConstIntPtr(void) { return &macroConstState; }
  #define MACRO_CONST_PTR macroConstIntPtr()
  #define MACRO_CONST_ADDRESS (&macroConstState)
  #define MACRO_TOP_CONST_PTR macroConstPtr
  #define MACRO_UNUSED_INVALID nonexistent_identifier)
  #define MACRO_UNUSED_RECURSIVE_A MACRO_UNUSED_RECURSIVE_B
  #define MACRO_UNUSED_RECURSIVE_B MACRO_UNUSED_RECURSIVE_A
}

assert(MACRO_TEXT.type == c_ptrConst(c_char));
assert(MACRO_CONST_PTR.type == c_ptrConst(c_int));
assert(MACRO_CONST_PTR[0] == 17);
assert(MACRO_CONST_ADDRESS.type == c_ptrConst(c_int));
assert(MACRO_TOP_CONST_PTR.type == c_ptr(c_int));
assert(MACRO_PTR.type == c_ptr(c_int));

writeln(MACRO_NESTED.type == c_ulong);
writeln(MACRO_SIZE);
writeln(MACRO_SELECT);
writeln(MACRO_TEXT[0] == 101:c_char);
writeln(MACRO_PTR[0]);
writeln(MACRO_RUNTIME);
writeln(MACRO_SELECT);
writeln(MACRO_PTR[0]);
