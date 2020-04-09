public use impl;
public use memTrackSupport;

config const doCorrectnessTest = true;

enum E { zero=-1, one=-2, two=-4, three=-8 };

inline proc writeMe(s) {
  if doCorrectnessTest then writeln(s);
}

use unitTest;
proc main() {
  doIt(string_impl);
  if totalMemLeaked > 0 then printMemAllocsByType();
}
