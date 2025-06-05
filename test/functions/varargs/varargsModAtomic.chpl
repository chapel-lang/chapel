// Taken from https://github.com/chapel-lang/chapel/issues/25858
proc myPrintln(args...)
{
  writeln("args.type     = ", args.type:string);
  writeln("args (before) = ", args);

  args[0].fetchAdd(10);  // modify tuple (just for test)

  writeln("args (after) = ", args);
}

var atomInt: atomic int = 1;
myPrintln(atomInt, 2.3, "four");
