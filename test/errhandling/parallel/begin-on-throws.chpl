use ExampleErrors;

proc test() {
  try {
    writeln("before sync block");
    sync {
      begin on Locales[numLocales-1] {
        throw new StringError("test error");
      }
      begin on Locales[numLocales-1] {
        throw new StringError("test error");
      }
    }
    writeln("after sync block");
  } catch errors: TaskErrors {
    for e in errors { 
      writeln("Caught group error e ", e.message());
    }
  } catch e {
    writeln("Caught other error ", e.message());
  }
}

test();
