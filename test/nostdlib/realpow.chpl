proc fun(x:real, y:real)
{
  return x**y;
}

proc main() {
  var x = 3; 
  var y = 2;
  var num = fun(x,y);
  var str = num:string;
  __primitive("chpl_error", str);
}

