proc test() {
  try {
    writeln("before sync block");
    sync {
      begin on Locales[numLocales-1] {
        throw new Error("test error");
      }
      begin on Locales[numLocales-1] {
        throw new Error("test error");
      }
    }
    writeln("after sync block");
  } catch errors: ErrorGroup {
    for e in errors { 
      writeln("Caught group error e ", e.msg);
    }
  } catch e {
    writeln("Caught other error ", e.msg);
  }
}

test();
