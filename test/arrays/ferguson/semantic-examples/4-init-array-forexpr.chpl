use samples;
var A:[1..2] R;
A[1] = one;
A[2] = two;

proc addOneReturn(ref r)
{
  r.x += 10;
  return r;
}

var B: [1..2] R = for r in A do addOneReturn(r);

writeln(B);

