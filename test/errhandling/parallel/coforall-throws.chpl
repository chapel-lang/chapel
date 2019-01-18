use ExampleErrors;

proc test() {
  try {
    writeln("before coforall block");
    coforall i in 1..2 {
      throw new StringError("test error");
    }
    writeln("after coforall block");
  } catch errors: TaskErrors {
    for e in errors { 
      writeln("Caught group error e ", e.message());
    }
  } catch e {
    writeln("Caught other error ", e.message());
  }
}

test();
