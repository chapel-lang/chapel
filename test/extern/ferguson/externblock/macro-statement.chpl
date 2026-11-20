use CTypes;

extern {
  static int macroState = 0;
  static int readMacroState(void) { return macroState; }
  #define MACRO_RESET() do { macroState = 10; macroState += 2; } while (0)
}

extern proc MACRO_RESET();

MACRO_RESET();
writeln(readMacroState());
