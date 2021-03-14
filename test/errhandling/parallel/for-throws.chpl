use ExampleErrors;

config const n = 1000;

proc test() {
  try {
    writeln("before for block");
    for i in 1..n {
      throw new owned StringError("test error");
    }
    writeln("after for block");
  } catch errors: TaskErrors {
    for e in errors { 
      if e != nil {
        writeln("Caught group error e ", e!.message());
      }
    }
  } catch e {
    writeln("Caught error ", e.message());
  }
}

test();
