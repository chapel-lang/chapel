use SysError;

config const i = 0;

proc deferThrow() throws {
  defer {
    throw new Error();
  }
}

proc deferTry() throws {
  defer {
    try {
      throw new Error();
    } catch e: UnexpectedEOFError {
      writeln("this shouldn't happen");
    }
  }
}

proc deferTryComplete() {
  defer {
    try {
      throw new UnexpectedEOFError();
    } catch {
      writeln("this is ok");
    }
  }
}

proc deferTryBang() {
  defer {
    try! {
      throw new Error();
    }
  }
}

if i == 0 then
  deferThrow();
if i == 1 then
  deferTry();
if i == 2 then
  deferTryComplete();
if i == 3 then
  deferTryBang();
