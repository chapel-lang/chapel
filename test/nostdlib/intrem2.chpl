proc fun(x:uint, y:uint)
{
  return x%y;
}

proc main() {
  var x = 17:uint; 
  var y = 7:uint;
  var num = fun(x,y);
  var str = num:string;
  __primitive("chpl_error", str);
}

