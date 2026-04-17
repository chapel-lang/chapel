// Test qualified call to ChapelBase function
// This used to cause 4 duplicate candidates due to re-export cycles

use ChapelBase;

proc main() {
  var result = ChapelBase.isTrue(true);
  writeln("Result: ", result);
}
