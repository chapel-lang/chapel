use ExampleErrors;

proc test() {
  try {
    writeln("before cobegin block");
    cobegin {
      throw new StringError("test error");
      throw new StringError("test error");
    }
    writeln("after cobegin block");
  } catch errors: TaskErrors {
    for e in errors { 
      writeln("Caught group error e ", e.message());
    }
  } catch e {
    writeln("Caught other error ", e.message());
  }
}

test();
