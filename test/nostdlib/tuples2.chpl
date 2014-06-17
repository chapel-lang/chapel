proc addone(len, x)
{
  var ret = x;
  for i in 1..len {
    ret[i] += 1;
  }
  return ret;
}

proc main() {
  var t = (1,2,3,4);
  var t2 = addone(t.size, t); 
  var str = t2:string;
  __primitive("chpl_error", str);
}

