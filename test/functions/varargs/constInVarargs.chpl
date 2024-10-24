// Taken from https://github.com/chapel-lang/chapel/issues/25858
proc myPrintln(const in args...)
{
  writeln("args.type     = ", args.type:string);
  writeln("args (before) = ", args);

  args[0] *= 10;  // should not be allowed

  writeln("args (after) = ", args);
}

myPrintln(1, 2.3, "four");
