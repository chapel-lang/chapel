proc fun(x:uint(8), y:uint(8))
{
  return x%y;
}

proc main() {
  var x = 17:uint(8);
  var y = 7:uint(8);
  var num = fun(x,y);
  var str = num:string;
  __primitive("chpl_error", str);
}

