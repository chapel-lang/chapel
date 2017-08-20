proc test() {
  try {
    writeln("before cobegin block");
    cobegin {
      throw new Error("test error");
      throw new Error("test error");
    }
    writeln("after cobegin block");
  } catch errors: ErrorGroup {
    for e in errors { 
      writeln("Caught group error e ", e.msg);
    }
  } catch e {
    writeln("Caught other error ", e.msg);
  }
}

test();
