// Taken from https://github.com/chapel-lang/chapel/issues/25858
proc myPrintln(args...)
{
  writeln("args.type     = ", args.type:string);
  writeln("args (before) = ", args);

  args[0][0] *= 10;  // modify tuple (just for test)

  writeln("args (after) = ", args);
}

var arr: [0..2] int = [1, 2, 3];
myPrintln(arr, 2.3, "four");
