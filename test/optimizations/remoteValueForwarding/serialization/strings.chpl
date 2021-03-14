
use CommDiagnostics;

config const n = 10;

const globalStr = "glob " * n;

proc print(const msg : string) {
  writeln();
  writeln("===== ", msg, " =====");
  for (loc, dat) in zip(Locales, getCommDiagnostics()) {
    writeln(loc, ": ", dat);
  }
  resetCommDiagnostics();
}


proc main() {
  const localStr = "loc " * n;

  on Locales.tail() {
    startCommDiagnostics();
    for s in globalStr {
      if s == "z" then halt();
    }
    stopCommDiagnostics();
  }
  print("global const string");

  on Locales.tail() {
    startCommDiagnostics();
    for s in localStr {
      if s == "z" then halt();
    }
    stopCommDiagnostics();
  }
  print("local string");

  const shortStr = "hi";

  // The point of testing a short string is to test an optimization where we
  // utilize a fixed-size buffer within the serialized data to avoid GET-ing
  // the remote buffer.
  startCommDiagnostics();
  on Locales.tail() {
    for s in shortStr {
      if s == "z" then halt();
    }
  }
  stopCommDiagnostics();
  print("local short string");
}
