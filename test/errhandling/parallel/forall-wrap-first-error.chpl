module ReThrown {

  config const useFirst = false;
  var count: atomic int;

  class UserError: Error {
    var x: int;
    proc init(x: int) {
      this.x = x;
      count.add(1);
    }
    proc deinit() {
      count.sub(1);
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
        if useFirst {
          throw errors.first();
        } else {
          for i in errors {
            throw i;
          }
        }
      }
    }
    catch e {
      writeln("Outer catch");
    }
    writeln("End of main count is ", count.read());
  }
} // module ReThrown
