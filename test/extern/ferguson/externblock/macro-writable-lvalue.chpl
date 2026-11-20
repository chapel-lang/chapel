use CTypes;

extern {
  static int macroLimit = 3;
  static int* macroLimitPtr(void) { return &macroLimit; }
  static int readMacroLimit(void) { return macroLimit; }
  #define MACRO_LIMIT (*macroLimitPtr())
}

extern var MACRO_LIMIT: c_int;

writeln(MACRO_LIMIT);
MACRO_LIMIT = 7;
writeln(readMacroLimit());
MACRO_LIMIT += 2;
writeln(readMacroLimit());
