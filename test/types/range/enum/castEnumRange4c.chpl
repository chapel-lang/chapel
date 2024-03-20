// We want to be able to differentiate between
// negative associated values and high unsigned associated values.
// For example, we may want the ability to obtain the associated value
// in its "natural" integral type.

enum eSigned   { x = 0, y = -1 };
enum eUnsigned { x = 0, y = max(uint) };

proc test(eValue, type idxType) {
  var r = eValue..eValue;
  try {
    writeln(r.tryCast(range(idxType)));
  } catch e {
    writeln(e);
  }
}

test(eSigned.y, int);     // -1
test(eSigned.y, uint);    // exception
test(eUnsigned.y, int);   // exception
test(eUnsigned.y, uint);  // max(uint)
