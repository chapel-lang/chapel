/*
This code tests the cases when the implementation of a required function
has an incorrect return type.
*/

interface IFC {
  proc reqSelf(formal1: Self): Self;
  proc reqInt(formal2: Self): int;
  proc dfltSelf(formal3: Self): Self {
    writeln("in dfltSelf");
    return formal3;
  }
  proc dfltInt(formal4: Self): int {
    writeln("in dfltInt");
    return 22;
  }
  proc implicitSelf(formal5: Self) {
    writeln("in implicitSelf");
    return formal5;
  }
  proc implicitInt(formal6: Self) {
    writeln("in implicitInt");
    return 33;
  }
}

implements IFC(string);

proc reqSelf(arg1: string): int return 5;
proc reqInt(arg2: string): string return "hi";
proc dfltSelf(arg3: string): int return 6;
proc dfltInt(arg4: string): string return "hii";
proc implicitSelf(arg5: string): int return 7;
proc implicitInt(arg6: string): string return "hiii";
