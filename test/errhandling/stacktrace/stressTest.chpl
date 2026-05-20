class MyError: Error {
  proc init(args...) do super.init((...args));
}
proc iThrow() throws {
  throw new MyError("oops");
}
proc deepCall(n: int) throws {
  if n > 0 then
    deepCall(n - 1);
  else
    iThrow();
}

config param doCatch = false;
config const depth = 1;
proc main() {
  try! deepCall(depth);
}
