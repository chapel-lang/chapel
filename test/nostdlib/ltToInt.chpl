proc lt(x:int, y:int):int
{
  return (x<y);
}

proc main() {
  var x = 7;
  var y = 17;
  var num = lt(x,y);
  var str = num:string;
  __primitive("chpl_error", str);
}

