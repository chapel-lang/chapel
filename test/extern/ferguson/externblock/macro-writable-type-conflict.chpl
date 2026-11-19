use CTypes;

extern {
  static long long macroState = 1;
  static long long* macroStatePtr(void) { return &macroState; }
  static long long readMacroState(void) { return macroState; }
  #define MACRO_WRONG_TYPE (*macroStatePtr())
}

extern var MACRO_WRONG_TYPE: real(64);

MACRO_WRONG_TYPE = 7.0;
writeln(readMacroState());
