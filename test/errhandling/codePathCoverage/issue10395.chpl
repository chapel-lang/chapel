class SampleError : Error {
  var msg:string;
  proc init(msg:string) {
    this.msg = msg;
  }
  override proc message() {
    return msg;
  }
}

proc thrower(cond: bool) : bool throws {
  if cond {
    return cond;
  }
  else {
    throw new owned SampleError("this wont compile");
  }
}

proc caller(cond: bool) {
  try! {
    writeln(thrower(cond));
  }
  catch e: SampleError {
    writeln(e.message());
  }
}

caller(false);
