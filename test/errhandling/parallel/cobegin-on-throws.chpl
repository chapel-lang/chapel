proc test() {
  try {
    writeln("before cobegin block");
    cobegin {
      {
        on Locales[numLocales-1] {
          throw new Error("test error");
        }
      }
      {
        on Locales[numLocales-1] {
          throw new Error("test error");
        }
      }
    }
    writeln("after cobegin block");
  } catch errors: TaskErrors {
    for e in errors { 
      writeln("Caught group error e ", e.msg);
    }
  } catch e {
    writeln("Caught other error ", e.msg);
  }
}

test();
