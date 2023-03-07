use BigInteger;
use Reflection;

config const executeLocale = 0;

const a = new bigint("123456789012345678901234567890");
const b = new bigint("123456789012345678901234567891");
const c = new bigint("246913578024691357802469135780");
const d = new bigint("-246913578024691357802469135780");

on Locales[min(Locales.size-1, executeLocale)] {
  assert((a+1) == b);
  assert((a+(1:bigint)) == b, getLineNumber());
  assert(b == (a+1));
  assert(b != a);
  assert(b > a);
  assert(b >= a);
  assert(!(b < a));
  assert(!(b <= a));

  assert((a*2) == c);
  assert((c-a) == a);
  assert(c == (a + a));
  assert((c+1) == (a+b));
  assert(d == -c);

  forall i in -10..10 do
    for j in -10..10 do
      if j != 0 {
        assert(div(i:bigint, j:bigint) == div(i,j));
        assert(mod(i:bigint, j:bigint) == mod(i,j));
        assert(mul(i:bigint, j:bigint) == mul(i,j));
      }

  proc div(a, b) {
    return a/b;
  }

  proc mod(a, b) {
    return a%b;
  }

  proc mul(a, b) {
    return a*b;
  }

  // Addition

  assert(a+(1:int(8)) == b);
  assert(a+(1:int(16)) == b);
  assert(a+(1:int(32)) == b);
  assert(a+(1:int(64)) == b);

  assert((1:int(8))+a == b);
  assert((1:int(16))+a == b);
  assert((1:int(32))+a == b);
  assert((1:int(64))+a == b);

  assert((-1:int(8))+b == a);
  assert((-1:int(16))+b == a);
  assert((-1:int(32))+b == a);
  assert((-1:int(64))+b == a);

  assert(b+(-1:int(8)) == a);
  assert(b+(-1:int(16)) == a);
  assert(b+(-1:int(32)) == a);
  assert(b+(-1:int(64)) == a);

  assert(a+(true) == b);
  assert(a+(1:uint(8)) == b);
  assert(a+(1:uint(16)) == b);
  assert(a+(1:uint(32)) == b);
  assert(a+(1:uint(64)) == b);

  assert((1:uint(8))+a == b);
  assert((1:uint(16))+a == b);
  assert((1:uint(32))+a == b);
  assert((1:uint(64))+a == b);

  // Subtraction

  assert(b-(1:int(8))  == a);
  assert(b-(1:int(16)) == a);
  assert(b-(1:int(32)) == a);
  assert(b-(1:int(64)) == a);

  assert((1:int(8))-b  == -a);
  assert((1:int(16))-b == -a);
  assert((1:int(32))-b == -a);
  assert((1:int(64))-b == -a);

  assert(a-(-1:int(8))  == b);
  assert(a-(-1:int(16)) == b);
  assert(a-(-1:int(32)) == b);
  assert(a-(-1:int(64)) == b);

  assert(b-(true) == a);
  assert((true)-b == -a);

  assert(b-(1:uint(8)) == a);
  assert(b-(1:uint(16)) == a);
  assert(b-(1:uint(32)) == a);
  assert(b-(1:uint(64)) == a);

  assert((1:uint(8))-b  == -a);
  assert((1:uint(16))-b == -a);
  assert((1:uint(32))-b == -a);
  assert((1:uint(64))-b == -a);

  // Multiplication
  assert(a*(1:int(8))  == a);
  assert(a*(1:int(16)) == a);
  assert(a*(1:int(32)) == a);
  assert(a*(1:int(64)) == a);

  assert((1:int(8))*a  == a);
  assert((1:int(16))*a == a);
  assert((1:int(32))*a == a);
  assert((1:int(64))*a == a);

  assert((-1:int(8))*a  == -a);
  assert((-1:int(16))*a == -a);
  assert((-1:int(32))*a == -a);
  assert((-1:int(64))*a == -a);

  assert(a*(-1:int(8))  == -a);
  assert(a*(-1:int(16)) == -a);
  assert(a*(-1:int(32)) == -a);
  assert(a*(-1:int(64)) == -a);

  assert(a*(true) == a);
  assert(a*(1:uint(8))  == a);
  assert(a*(1:uint(16)) == a);
  assert(a*(1:uint(32)) == a);
  assert(a*(1:uint(64)) == a);

  assert((1:uint(8))*a  == a);
  assert((1:uint(16))*a == a);
  assert((1:uint(32))*a == a);
  assert((1:uint(64))*a == a);

  // Bit shifts
  assert(5:bigint  << 3  == 40);
  assert(5:bigint  >> 1  == 2);
  assert(-5:bigint << 3  == -40);
  assert(-5:bigint >> 1  == -3);
  assert(5:bigint  >> -3  == 40);
  assert(5:bigint  << -1  == 2);
  assert(-5:bigint >> -3  == -40);
  assert(-5:bigint << -1  == -3);

  assert(5:bigint  << 3:uint  == 40);
  assert(5:bigint  >> 1:uint  == 2);
  assert(-5:bigint << 3:uint  == -40);
  assert(-5:bigint >> 1:uint  == -3);

  // Boolean ops
  assert(~123:bigint == -124);
  assert(123:bigint & 234:bigint == 106);
  assert(123:bigint | 234:bigint == 251);

  var ret:bigint;
  ret.gcd(48:bigint, 180:bigint);
  assert(ret == 12);
  ret.lcm(48:bigint, 180:bigint);
  assert(ret == 720);

  ret.fac(40);
  assert(ret == "815915283247897734345611269596115894272000000000":bigint);

  var la = "315135":bigint;
  var lb = "12412":bigint;

  ret.xor(la,lb);
  assert(ret == 327299);
  assert(la & lb == 124);
  assert(la | lb == 327423);

  assert((90:bigint).sizeInBase(10) == 3);
  assert((99:bigint).sizeInBase(10) == 3);

  ret.sqrt(4:bigint);
  assert(ret == 2);
  ret.sqrt(5:bigint);
  assert(ret == 2);

  assert((6:bigint)%(5:bigint) == 6%5);

  // Conversions
  use Random;
  var randStream = new RandomStream(int);
  var randVal = randStream.getNext();
  assert(randVal:bigint == randVal);

  var uRandStream = new RandomStream(uint);
  var uRandVal = uRandStream.getNext();
  assert(uRandVal:bigint == uRandVal);

  assert(a.cmp(b) == -1);
  assert(b.cmp(a) == 1);
  assert(a.cmp(a) == 0);
}