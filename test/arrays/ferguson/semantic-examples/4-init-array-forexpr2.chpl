use samples;
var A:[1..2] R;
A[1] = one;
A[2] = two;

proc addOneReturn(ref r)
{
  r.x += 10;
  return r;
}

var B = for r in A do addOneReturn(r);

writeln(B);

