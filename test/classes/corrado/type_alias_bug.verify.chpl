class E1: Error {
  proc init(msg: string) do super.init(msg);
}
type E2 = E1;

proc throwingProc() throws {
  throw new E2("an error occurred");
}

try {
  throwingProc();
} catch e: E2 {
  writeln(e);
} catch e {
  writeln("wrong error type: ", e);
}
