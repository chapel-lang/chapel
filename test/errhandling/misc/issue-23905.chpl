config const trials = 1;

proc main() {
  for 1..trials {
    var started: atomic int;
    try {
      func(started);
      halt("expected TaskErrors");
    } catch e {
      checkErrors(e: borrowed TaskErrors, started.read());
    }
  }
  writeln("all errors accounted for");
}

proc checkErrors(group: borrowed TaskErrors, expected: int) {
  var actual = 0;
  for err in group {
    if err!.message() != "in forall" then
      halt("unexpected task error: ", err!.message());
    actual += 1;
  }
  if group.nErrors != expected || actual != expected then
    halt("expected ", expected, " errors, got ", actual,
         " of ", group.nErrors, " slots");
}

proc func(ref started: atomic int) throws {
  coforall loc in Locales {
    on loc {
      var localStarted: atomic int;
      try {
        forall i in 0..8 {
          localStarted.add(1);
          started.add(1);
          throw new Error("in forall");
        }
      } catch e {
        checkErrors(e: borrowed TaskErrors, localStarted.read());
        throw e;
      }
    }
  }
}
