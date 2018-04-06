use ExampleErrors;

class MyError : Error {
  var msg:string = "";
  var x = 1;
  proc init(x:int) {
    this.msg = x:string;
    this.x = x;
  }
  proc init(msg:string) {
    this.msg = msg;
    this.x = 1;
  }
}

proc test() {
  try {
    writeln("before coforall block");
    coforall i in 1..3 {
      if i < 3 then
        throw new MyError("test error");
      else
        throw new StringError("other error");
    }
    writeln("after coforall block");
  } catch errors: TaskErrors {
    var hasMyError = errors.contains(MyError);
    writeln("contains MyError? ", hasMyError);
    writeln("iterating MyError:");
    for error in errors.filter(MyError) {
      writeln(error.x);
    }
    writeln("iterating everything else:");
    for error in errors {
      var e = error:MyError;
      if e == nil then
        writeln(error);
    }
  } catch e {
    writeln("Caught other error ", e.message());
  }
}

test();
