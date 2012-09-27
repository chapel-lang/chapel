proc addone(len, x)
{
  var ret = x;
  for i in 1..len {
    ret[i] += 1;
  }
  return ret;
}

proc main() {
  var t = (1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24);
  var t2 = addone(t.size, t); 
  var str = t2:string;
  __primitive("chpl_error", str);
}

