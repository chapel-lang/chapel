use SysCTypes;

export proc f1(x : real) : real {
  return x;
}
export proc f2() {
}

config const option = true;

proc main() {
  var ptr = c_ptrTo(f1);
  if option then
    ptr = c_ptrTo(f2); // expecting type error
}
