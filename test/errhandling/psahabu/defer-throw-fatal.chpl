use SysError;

config const i = 0;

proc deferThrow() throws {
  defer {
    throw new owned Error();
  }
}

proc deferTry() throws {
  defer {
    try {
      throw new owned Error();
    } catch e: UnexpectedEOFError {
      writeln("this shouldn't happen");
    }
  }
}

proc deferTryComplete() {
  defer {
    try {
      throw new owned UnexpectedEOFError();
    } catch {
      writeln("this is ok");
    }
  }
}

proc deferTryBang() {
  defer {
    try! {
      throw new owned Error();
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
