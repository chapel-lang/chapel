proc test() {
  try {
    writeln("before coforall block");
    coforall i in 1..2 {
      on Locales[numLocales-1] {
        throw new Error("test error");
      }
    }
    writeln("after coforall block");
  } catch errors: TaskErrors {
    for e in errors { 
      writeln("Caught group error e ", e.msg);
    }
  } catch e {
    writeln("Caught other error ", e.msg);
  }
}

test();
