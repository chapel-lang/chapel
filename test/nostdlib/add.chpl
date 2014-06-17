proc add(x:int, y:int)
{
  return x+y;
}

proc main() {
  var x = 7;
  var y = 17;
  var sum = add(x,y);
  var str = sum:string;
  __primitive("chpl_error", str);
}

