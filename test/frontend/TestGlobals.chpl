
use Print;

// Test uses of global variables in internal/standard modules.
// This needs to work to enable partial progress on the converter.

proc main() {
  var x = c_sublocid_none;
  println(x); // -1

  println(defaultHashTableResizeThreshold); // 0.5
}
