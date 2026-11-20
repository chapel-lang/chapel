use CTypes;

extern {
  static short macroSlots[2] = {3, 5};
  static int macroSlot = 0;
  static int macroLookups = 0;
  static void selectMacroSlot(int slot) { macroSlot = slot; }
  static short readMacroSlot(int slot) { return macroSlots[slot]; }
  static int readMacroLookups(void) { return macroLookups; }
  static short* macroSlotPtr(void) {
    macroLookups++;
    return &macroSlots[macroSlot];
  }
  #define MACRO_SLOT (*macroSlotPtr())
  #define MACRO_SMALL ((short)7)
}

proc addToSlot(ref value: c_short) {
  value += 1;
}

proc readSlot(): c_short {
  return MACRO_SLOT;
}

assert(MACRO_SLOT.type == c_short);
assert(MACRO_SMALL.type == c_short);
assert(MACRO_SMALL == 7);
assert(readSlot() == 3);
MACRO_SLOT = 10;
addToSlot(MACRO_SLOT);
var first = c_ptrTo(MACRO_SLOT);
selectMacroSlot(1);
assert(readSlot() == 5);
var lookups = readMacroLookups();
MACRO_SLOT += 2;
assert(readMacroLookups() == lookups + 1);
assert(first.deref() == 11);
assert(readMacroSlot(0) == 11);
assert(readMacroSlot(1) == 7);
selectMacroSlot(0);
assert(readSlot() == 11);
writeln("macro lvalue locations passed");
