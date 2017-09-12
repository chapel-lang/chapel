config const n = 1000;

proc test() {
  try {
    writeln("before forall block");
    forall i in 1..n {
      if i == 1 then
        throw new Error("test error");
    }
    writeln("after forall block");
  } catch errors: TaskErrors {
    for e in errors { 
      writeln("Caught group error e ", e.msg);
    }
  } catch e {
    writeln("Caught other error ", e.msg);
  }
}

test();
