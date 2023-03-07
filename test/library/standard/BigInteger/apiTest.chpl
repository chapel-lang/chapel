use BigInteger;
use Reflection;

config const executeLocale = 0;

const a = new bigint("123456789012345678901234567890");
const b = new bigint("123456789012345678901234567891");
const c = new bigint("246913578024691357802469135780");
const d = new bigint("-246913578024691357802469135780");

on Locales[min(Locales.size-1, executeLocale)] {
  writeIfFalse((a+1) == b, getLineNumber());
  writeIfFalse((a+(1:bigint)) == b, getLineNumber());
  writeIfFalse(b == (a+1), getLineNumber());
  writeIfFalse(b != a, getLineNumber());
  writeIfFalse(b > a, getLineNumber());
  writeIfFalse(b >= a, getLineNumber());
  writeIfFalse(!(b < a), getLineNumber());
  writeIfFalse(!(b <= a), getLineNumber());

  writeIfFalse((a*2) == c, getLineNumber());
  writeIfFalse((c-a) == a, getLineNumber());
  writeIfFalse(c == (a + a), getLineNumber());
  writeIfFalse((c+1) == (a+b), getLineNumber());
  writeIfFalse(d == -c, getLineNumber());

  forall i in -10..10 do
    for j in -10..10 do
      if j != 0 {
        writeIfFalse(div(i:bigint, j:bigint) == div(i,j), getLineNumber());
        writeIfFalse(mod(i:bigint, j:bigint) == mod(i,j), getLineNumber());
        writeIfFalse(mul(i:bigint, j:bigint) == mul(i,j), getLineNumber());
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

  writeIfFalse(a+(1:int(8)) == b, getLineNumber());
  writeIfFalse(a+(1:int(16)) == b, getLineNumber());
  writeIfFalse(a+(1:int(32)) == b, getLineNumber());
  writeIfFalse(a+(1:int(64)) == b, getLineNumber());

  writeIfFalse((1:int(8))+a == b, getLineNumber());
  writeIfFalse((1:int(16))+a == b, getLineNumber());
  writeIfFalse((1:int(32))+a == b, getLineNumber());
  writeIfFalse((1:int(64))+a == b, getLineNumber());

  writeIfFalse((-1:int(8))+b == a, getLineNumber());
  writeIfFalse((-1:int(16))+b == a, getLineNumber());
  writeIfFalse((-1:int(32))+b == a, getLineNumber());
  writeIfFalse((-1:int(64))+b == a, getLineNumber());

  writeIfFalse(b+(-1:int(8)) == a, getLineNumber());
  writeIfFalse(b+(-1:int(16)) == a, getLineNumber());
  writeIfFalse(b+(-1:int(32)) == a, getLineNumber());
  writeIfFalse(b+(-1:int(64)) == a, getLineNumber());

  writeIfFalse(a+(true) == b, getLineNumber());
  writeIfFalse(a+(1:uint(8)) == b, getLineNumber());
  writeIfFalse(a+(1:uint(16)) == b, getLineNumber());
  writeIfFalse(a+(1:uint(32)) == b, getLineNumber());
  writeIfFalse(a+(1:uint(64)) == b, getLineNumber());

  writeIfFalse((1:uint(8))+a == b, getLineNumber());
  writeIfFalse((1:uint(16))+a == b, getLineNumber());
  writeIfFalse((1:uint(32))+a == b, getLineNumber());
  writeIfFalse((1:uint(64))+a == b, getLineNumber());

  // Subtraction

  writeIfFalse(b-(1:int(8))  == a, getLineNumber());
  writeIfFalse(b-(1:int(16)) == a, getLineNumber());
  writeIfFalse(b-(1:int(32)) == a, getLineNumber());
  writeIfFalse(b-(1:int(64)) == a, getLineNumber());

  writeIfFalse((1:int(8))-b  == -a, getLineNumber());
  writeIfFalse((1:int(16))-b == -a, getLineNumber());
  writeIfFalse((1:int(32))-b == -a, getLineNumber());
  writeIfFalse((1:int(64))-b == -a, getLineNumber());

  writeIfFalse(a-(-1:int(8))  == b, getLineNumber());
  writeIfFalse(a-(-1:int(16)) == b, getLineNumber());
  writeIfFalse(a-(-1:int(32)) == b, getLineNumber());
  writeIfFalse(a-(-1:int(64)) == b, getLineNumber());

  writeIfFalse(b-(true) == a, getLineNumber());
  writeIfFalse((true)-b == -a, getLineNumber());

  writeIfFalse(b-(1:uint(8)) == a, getLineNumber());
  writeIfFalse(b-(1:uint(16)) == a, getLineNumber());
  writeIfFalse(b-(1:uint(32)) == a, getLineNumber());
  writeIfFalse(b-(1:uint(64)) == a, getLineNumber());

  writeIfFalse((1:uint(8))-b  == -a, getLineNumber());
  writeIfFalse((1:uint(16))-b == -a, getLineNumber());
  writeIfFalse((1:uint(32))-b == -a, getLineNumber());
  writeIfFalse((1:uint(64))-b == -a, getLineNumber());

  // Multiplication
  writeIfFalse(a*(1:int(8))  == a, getLineNumber());
  writeIfFalse(a*(1:int(16)) == a, getLineNumber());
  writeIfFalse(a*(1:int(32)) == a, getLineNumber());
  writeIfFalse(a*(1:int(64)) == a, getLineNumber());

  writeIfFalse((1:int(8))*a  == a, getLineNumber());
  writeIfFalse((1:int(16))*a == a, getLineNumber());
  writeIfFalse((1:int(32))*a == a, getLineNumber());
  writeIfFalse((1:int(64))*a == a, getLineNumber());

  writeIfFalse((-1:int(8))*a  == -a, getLineNumber());
  writeIfFalse((-1:int(16))*a == -a, getLineNumber());
  writeIfFalse((-1:int(32))*a == -a, getLineNumber());
  writeIfFalse((-1:int(64))*a == -a, getLineNumber());

  writeIfFalse(a*(-1:int(8))  == -a, getLineNumber());
  writeIfFalse(a*(-1:int(16)) == -a, getLineNumber());
  writeIfFalse(a*(-1:int(32)) == -a, getLineNumber());
  writeIfFalse(a*(-1:int(64)) == -a, getLineNumber());

  writeIfFalse(a*(true) == a, getLineNumber());
  writeIfFalse(a*(1:uint(8))  == a, getLineNumber());
  writeIfFalse(a*(1:uint(16)) == a, getLineNumber());
  writeIfFalse(a*(1:uint(32)) == a, getLineNumber());
  writeIfFalse(a*(1:uint(64)) == a, getLineNumber());

  writeIfFalse((1:uint(8))*a  == a, getLineNumber());
  writeIfFalse((1:uint(16))*a == a, getLineNumber());
  writeIfFalse((1:uint(32))*a == a, getLineNumber());
  writeIfFalse((1:uint(64))*a == a, getLineNumber());

  // Bit shifts
  writeIfFalse(5:bigint  << 3  == 40, getLineNumber());
  writeIfFalse(5:bigint  >> 1  == 2, getLineNumber());
  writeIfFalse(-5:bigint << 3  == -40, getLineNumber());
  writeIfFalse(-5:bigint >> 1  == -3, getLineNumber());
  writeIfFalse(5:bigint  >> -3  == 40, getLineNumber());
  writeIfFalse(5:bigint  << -1  == 2, getLineNumber());
  writeIfFalse(-5:bigint >> -3  == -40, getLineNumber());
  writeIfFalse(-5:bigint << -1  == -3, getLineNumber());

  writeIfFalse(5:bigint  << 3:uint  == 40, getLineNumber());
  writeIfFalse(5:bigint  >> 1:uint  == 2, getLineNumber());
  writeIfFalse(-5:bigint << 3:uint  == -40, getLineNumber());
  writeIfFalse(-5:bigint >> 1:uint  == -3, getLineNumber());

  // Boolean ops
  writeIfFalse(~123:bigint == -124, getLineNumber());
  writeIfFalse(123:bigint & 234:bigint == 106, getLineNumber());
  writeIfFalse(123:bigint | 234:bigint == 251, getLineNumber());

  var ret:bigint;
  ret.gcd(48:bigint, 180:bigint);
  writeIfFalse(ret == 12, getLineNumber());
  ret.lcm(48:bigint, 180:bigint);
  writeIfFalse(ret == 720, getLineNumber());

  ret.fac(40);
  writeIfFalse(ret == "815915283247897734345611269596115894272000000000":bigint, getLineNumber());

  var la = "315135":bigint;
  var lb = "12412":bigint;

  ret.xor(la,lb);
  writeIfFalse(ret == 327299, getLineNumber());
  writeIfFalse(la & lb == 124, getLineNumber());
  writeIfFalse(la | lb == 327423, getLineNumber());

  writeIfFalse((90:bigint).sizeInBase(10) == 3, getLineNumber());
  writeIfFalse((99:bigint).sizeInBase(10) == 3, getLineNumber());

  ret.sqrt(4:bigint);
  writeIfFalse(ret == 2, getLineNumber());
  ret.sqrt(5:bigint);
  writeIfFalse(ret == 2, getLineNumber());

  writeIfFalse((6:bigint)%(5:bigint) == 6%5, getLineNumber());

  // Conversions
  use Random;
  var randStream = new RandomStream(int);
  var randVal = randStream.getNext();
  writeIfFalse(randVal:bigint == randVal, getLineNumber());

  var uRandStream = new RandomStream(uint);
  var uRandVal = uRandStream.getNext();
  writeIfFalse(uRandVal:bigint == uRandVal, getLineNumber());

  writeIfFalse(a.cmp(b) == -1, getLineNumber());
  writeIfFalse(b.cmp(a) == 1, getLineNumber());
  writeIfFalse(a.cmp(a) == 0, getLineNumber());
}