
proc throwError(i) throws {
  if i == 1 then
    throw new Error("abc");
}

proc test() {
    writeln("before forall block");
    forall idx in 1..2 {
      throwError(idx);
    }
    writeln("after forall block");
}

test();
