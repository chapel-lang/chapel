class UserError: Error {
  var msg: string;

  proc init(s: string) {
    msg = s;
  }
  proc deinit() {
    writeln("deinit called for UserError('", msg, "')");
  }
  override proc message() {
    return "UserError: " + msg;
  }
}

class Logger: Error {
  var x;

  proc init(in x: owned) {
    this.x = x;
    writeln("Logger inited with type ", this.x.type:string);
  }
  proc deinit() {
    writeln("deinit called for Logger");
  }
  override proc message() { return "[DateHere TimeHere] " + x.message(); }
}

proc main() {
  try {
    try {
      throw new owned UserError("Something failed.");
    } catch e: UserError {
      throw new owned Logger(e);
    }
  } catch e {
    writeln(e);
  }
}
