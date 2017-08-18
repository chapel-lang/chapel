config const n = 1000;

proc test() {
  try {
    writeln("before for block");
    for i in 1..n {
      throw new Error("test error");
    }
    writeln("after for block");
  } catch errors: ErrorGroup {
    for e in errors { 
      writeln("Caught group error e ", e.msg);
    }
  } catch e {
    writeln("Caught error ", e.msg);
  }
}

test();
