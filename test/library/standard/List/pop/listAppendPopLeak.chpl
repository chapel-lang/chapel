private use List;

config const testIters = 8;

proc main() {
  var s: string;
  for i in 1..10 do s += i:string;

  // Start test in new scope to avoid firing `s` destructor first.
  {
    var lines: list(string);
   
    // Repeated pushBack/getAndRemove of the same index should not leak.
    for i in 1..testIters {
      lines.pushBack(s);
      lines.getAndRemove(0);
    }
  }
}
