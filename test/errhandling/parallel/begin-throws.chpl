proc test() {
  try {
    writeln("before sync block");
    sync {
      begin {
        throw new Error("test error");
      }
      begin {
        throw new Error("test error");
      }
    }
    writeln("after sync block");
  } catch errors: TaskErrors {
    for e in errors { 
      writeln("Caught group error e ", e.msg);
    }
  } catch e {
    writeln("Caught other error ", e.msg);
  }
}

test();
