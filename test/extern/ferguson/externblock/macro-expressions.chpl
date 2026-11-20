use CTypes;

extern {
  static int macroState = 41;
  #define MACRO_PTR (&macroState)
  #define MACRO_NESTED ((unsigned long)(1 + 2 * 3))
  #define MACRO_RUNTIME (++macroState)
  #define MACRO_SIZE sizeof(macroState)
  #define MACRO_SELECT (macroState == 41 ? 3 : 4)
  #define MACRO_TEXT (&"hello"[1])
  #define MACRO_UNUSED_INVALID nonexistent_identifier)
  #define MACRO_UNUSED_RECURSIVE_A MACRO_UNUSED_RECURSIVE_B
  #define MACRO_UNUSED_RECURSIVE_B MACRO_UNUSED_RECURSIVE_A
}

writeln(MACRO_NESTED.type == c_ulong);
writeln(MACRO_SIZE);
writeln(MACRO_SELECT);
writeln(MACRO_TEXT[0] == 101:c_char);
writeln(MACRO_PTR[0]);
writeln(MACRO_RUNTIME);
writeln(MACRO_SELECT);
writeln(MACRO_PTR[0]);
