class UserError: Error, writeSerializable {
  var msg: string;
  proc init(s: string) { msg = s; }
  proc postinit() { writeln("init UserError"); }
  proc deinit() { writeln("deinit UserError"); }
  override proc serialize(writer, ref serializer) throws { writer.write("UserError: ", msg); }
}
class Logger: Error, writeSerializable {
  var x;
  proc init(x) { this.x = x; }
  proc postinit() { writeln("init Logger"); }
  proc deinit() { writeln("deinit Logger"); }
  override proc serialize(writer, ref serializer) throws { writer.write("[Date Time] ", x, " type:", x.type:string); }
}
proc main() {
  try {
    try { throw new owned UserError("Failure!"); }
    catch e: UserError {
      throw new owned Logger(e.borrow()); // What is going on here?
    }
  } catch e {
    writeln(e);
  }
}
