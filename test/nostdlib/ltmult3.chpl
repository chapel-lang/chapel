proc fun(x:real, y:real, n:uint)
{
  return (x < y) * 4 / n:real;
}

proc main() {
  var num = fun(0.7, 0.8, 2);
  var str = num:string;
  __primitive("chpl_error", str);
}

