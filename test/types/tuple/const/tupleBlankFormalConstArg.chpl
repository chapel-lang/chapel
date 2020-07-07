record r { var x: int = 0; }

// Default intent for record element should be `const ref` or `ref if
// modified` (can't remember which one yet)...If it's `const ref` then
// the assignment in the body should fail. If it's `ref if modified`
// then it should fail at the callsite since the actual is const (?).
// TODO: Check if default intent for some tuple elements is 'ref if
// modified' or not.
proc modifyConstActualViaDefaultIntentArg(tup) {
  tup[1].x = 256;
}

proc test() {
  const tup = (128, new r(0));
  writeln(tup);
  modifyConstActualViaDefaultIntentArg(tup);
  writeln(tup);
  return;
}
test();

