// Taken from https://github.com/chapel-lang/chapel/issues/25858
proc myPrintln(args...)
{
  writeln("args.type     = ", args.type:string);
  writeln("args (before) = (", args[0].readFF(), ", ", args[1], ", ", args[2],
          ")");

  args[0].writeFF(10);  // modify tuple (just for test)

  writeln("args (after) = (", args[0].readFF(), ", ", args[1], ", ", args[2],
          ")");
}

var syncInt: sync int = 1;
myPrintln(syncInt, 2.3, "four");
