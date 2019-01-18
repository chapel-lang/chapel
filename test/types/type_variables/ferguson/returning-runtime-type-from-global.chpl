var D = {6};
proc getTypeFromLocal() type {
  var A:[D] int;
  return A.type;
}
proc test() {
  var B: getTypeFromLocal();
  D += 2;
  D -= 6;
  writeln(B.domain);
  writeln(B);
  // should it output {6} or {2} ?
  // A domain returns by value from a function. Why does
  // a domain in a run-time type return by reference?
}
test();

