module ReThrown {
class UserError: Error {
  var x: int;
  proc init(x: int) {
    this.x = x;
    writeln("UserError.init");
  }
  proc deinit() {
    writeln("UserError.deinit");
  }
}
proc main() {
  try {
    try {
      forall i in 0..3 {
        var ex = new owned UserError(1);
        throw ex;
      }
    }
    catch errors: TaskErrors {
      writeln("Caught some TaskErrors");
      for i in errors {
        //compilerError(i.type:string);
        throw i;
      }
    }
  }
  catch e {
    writeln("Outer catch");
  }
  writeln("End of main");
}
} // module ReThrown
