proc fp754putZ(v : uint(?w)) : real(w)
   where w == 32 || w == 64
{
  var x:real(w) = v:real(w);
  writeln(v.type:string);
  writeln(x.type:string);
  return x;
}

proc test() {
  type R = real(32);
  type U = uint(numBits(R));
  const zero = 0:U;

  writeln(fp754putZ(zero));
}

test();
