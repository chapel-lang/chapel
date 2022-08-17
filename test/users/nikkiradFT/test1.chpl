//@unstable "This module is unstable and its interface is subject to change in the future."
use GPUDiagnostics;





module Test1 {
    //unstable "The variable 'msg' is unstable"
    config const msg: string = "This is test 1";
}

proc main() {
    writeln(msg);
}


/*
@unstable "This module is unstable!"
module M {
  var x: int;
}
module N {
  use M; // This will trigger the unstable warning

  proc main() {
    writeln(x); // This probably wouldn't, because it doesn't directly reference M
    writeln(M.x); // But this would, since it says M in it
  }
}
*/