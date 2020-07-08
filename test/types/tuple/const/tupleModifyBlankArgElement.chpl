record r { var x: int = 0; }

// When a tuple formal has blank intent, record elements are passed by
// `const ref`. Any modification of `tup[1]` should emit an error.
proc modifyBlankArgElement(tup) {
  tup[1].x = 256;
}

proc test() {
  const tup = (128, new r(0));
  writeln(tup);
  modifyBlankArgElement(tup);
  writeln(tup);
  return;
}
test();

