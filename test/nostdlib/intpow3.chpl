proc fun(x:uint, y:uint)
{
  return x**y;
}

proc main() {
  var x = 3:uint;
  var y = 2:uint;
  var num = fun(x,y);
  var str = num:string;
  __primitive("chpl_error", str);
}

