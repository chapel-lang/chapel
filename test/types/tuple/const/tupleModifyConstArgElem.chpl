record r { var x: int = 0; }

// Tuple is marked `const`, which means element 1 is passed by `const ref`.
// Any modification of `tup[1]` should emit an error.
proc modifyConstArgElement(const tup) {
  tup[1].x = 256;
}

proc test() {
  const tup = (128, new r(0));
  writeln(tup);
  modifyConstArgElement(tup);
  writeln(tup);
  return;
}
test();

