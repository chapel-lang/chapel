proc f(inout arg) {
  compilerWarning("in f, arg.type is ", arg.type:string);
  writeln("in f, arg= ", arg);
  arg = 1;
}
{
  var A:[1..10] int;
  ref slice = A[1..3];
  f(slice);
  writeln(slice);
  writeln(A);
}
