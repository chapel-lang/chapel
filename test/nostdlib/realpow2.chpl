proc fun(x:real, y:real)
{
  return x**y;
}

proc main() {
  var x = 9;
  var y = 0.5;
  var num = fun(x,y);
  var str = num:string;
  __primitive("chpl_error", str);
}

