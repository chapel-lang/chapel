// Check if fast flag is generated for float operations

proc f(x,y)
{
  var a1 = x+y;
  //CHECK: fadd fast
  var a2 = x-y;
  //CHECK: fsub fast
  var a3 = x*y;
  //CHECK: fmul fast
  var a4 = x/y;
  //CHECK: fdiv fast
  var res = a1+a2+a3+a4;
  //CHECK: fcmp fast
  if(a1 > 1.0)
  {
    return res;
  }

  return res*1.5;
}

writeln(f(10.0, 5.0));
