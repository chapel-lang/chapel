// Code from issue #10224 opened by BryantLam on Github
module ExceptionWarning {
  proc main() {
    try {
      try {
        throw new owned NilThrownError();
      }
      catch e1: NilThrownError {
        writeln("Caught NilThrownError");
        throw e1;
      }
    }
    catch e2 {
      writeln("Caught general error");
    }
  }
}
