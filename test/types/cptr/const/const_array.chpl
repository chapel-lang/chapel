use CTypes;

proc external(arg : c_ptrConst(real)) {
  for i in 0..2 {
    writeln(arg[i]);
  }
}

proc main()
{
  const x = [1.1, 2.2, 3.3];

  external(c_ptrToConst(x)); // explicitly using const pointer
  var y = x;
  external(c_ptrTo(y)); // using a non-const pointer as const
}
