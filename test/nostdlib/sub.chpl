proc sub(x:int, y:int)
{
  return x-y;
}

proc main() {
  var x = 7; 
  var y = 17;
  var res = sub(x,y);
  var str = res:string;
  __primitive("chpl_error", str);
}

