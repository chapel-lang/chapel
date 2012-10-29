proc fun(x:real, y:real)
{
  return x/y;
}

proc main() {
  var x = 17; 
  var y = 7;
  var num = fun(x,y);
  var str = num:string;
  __primitive("chpl_error", str);
}

