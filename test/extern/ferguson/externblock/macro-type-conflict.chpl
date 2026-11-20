use CTypes;

extern {
  static int macroState = 1;
  #define MACRO_WRONG_TYPE (macroState + 1)
}

extern const MACRO_WRONG_TYPE: c_longlong;

writeln(MACRO_WRONG_TYPE);
