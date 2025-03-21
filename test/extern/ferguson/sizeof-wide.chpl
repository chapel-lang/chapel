use CTypes;

extern proc sizeof(x):c_size_t;

class C {
  var x:int;
}

proc main() {
  var c:unmanaged C?;
  on Locales[numLocales-1] {
    c = new unmanaged C(1);
  }
  // Expect this to be 8 because it assumes 'unmanaged C' is not wide
  writeln(c_sizeof(unmanaged C));
  // Expect this to be 16 because 'c' is a wide pointer
  writeln(sizeof(c));
  delete c;
}
