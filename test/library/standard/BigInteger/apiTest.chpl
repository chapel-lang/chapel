use BigInteger;

config const executeLocale = 0;

const a = new bigint("123456789012345678901234567890");
const b = new bigint("123456789012345678901234567891");
const c = new bigint("246913578024691357802469135780");
const d = new bigint("-246913578024691357802469135780");

var aa = "315135":bigint;
var bb = "12412":bigint;
var cc = "3426495623485904783478347":bigint;
var dd = "-1398984130":bigint;
var ff = "2413804710837418037418307081437315263635345357386985747464":bigint;
var gg = "-1":bigint;

var la = "315135":bigint;
var lb = "12412":bigint;

var s:bigint;

on Locales[min(Locales.domain.high, executeLocale)] {
  assert((a+1) == b);
  assert((a+(1:bigint)) == b);
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
        assert(i:bigint/j:bigint == i/j);
        assert(i:bigint%j:bigint == i%j);
        assert(i:bigint*j:bigint == i*j);
      }

  // Addition

  assert(a+(1:int(8))  == b);
  assert(a+(1:int(16)) == b);
  assert(a+(1:int(32)) == b);
  assert(a+(1:int(64)) == b);

  assert((1:int(8))+a  == b);
  assert((1:int(16))+a == b);
  assert((1:int(32))+a == b);
  assert((1:int(64))+a == b);

  assert((-1:int(8))+b  == a);
  assert((-1:int(16))+b == a);
  assert((-1:int(32))+b == a);
  assert((-1:int(64))+b == a);

  assert(b+(-1:int(8))  == a);
  assert(b+(-1:int(16)) == a);
  assert(b+(-1:int(32)) == a);
  assert(b+(-1:int(64)) == a);

  assert(a+(true) == b);
  assert(a+(1:uint(8))  == b);
  assert(a+(1:uint(16)) == b);
  assert(a+(1:uint(32)) == b);
  assert(a+(1:uint(64)) == b);

  assert((1:uint(8))+a  == b);
  assert((1:uint(16))+a == b);
  assert((1:uint(32))+a == b);
  assert((1:uint(64))+a == b);

  // unary
  assert(+a == a);


  // assign ops
  s = a;
  s += 1;
  assert(s == b);
  s = a;
  s += 1:bigint;
  assert(s == b);
  s = a;
  s += (true);
  assert(s == b);

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

  assert(b-(1:uint(8))  == a);
  assert(b-(1:uint(16)) == a);
  assert(b-(1:uint(32)) == a);
  assert(b-(1:uint(64)) == a);

  assert((1:uint(8))-b  == -a);
  assert((1:uint(16))-b == -a);
  assert((1:uint(32))-b == -a);
  assert((1:uint(64))-b == -a);

  // unary
  assert(-(17:bigint) == -17:bigint);

  // assign ops
  s = b;
  s -= 1;
  assert(s == a);
  s = b;
  s -= 1:bigint;
  assert(s == a);
  s = b;
  s -= (true);
  assert(s == a);

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

  // assign ops
  s = a;
  s *= 1;
  assert(s == a);
  s = a;
  s *= 1:bigint;
  assert(s == a);
  s = a;
  s *= true;
  assert(s == a);

  assert(aa + bb == 327547);
  assert(aa + bb + cc == "3426495623485904783805894":bigint);
  assert(aa + bb + cc + dd == "3426495623485903384821764":bigint);
  assert(aa + bb + cc + dd + ff == "2413804710837418037418307081437318690130968843290370569228":bigint);
  assert(aa + bb + cc + dd + ff + gg == "2413804710837418037418307081437318690130968843290370569227":bigint);

  assert(aa * bb == "3911455620":bigint);
  assert(aa * bb * cc == "13402585563389346256121263521460140":bigint);
  assert(aa * bb * cc * dd == "-18750004504148804423388563022070650287578200":bigint);
  assert(aa * bb * cc * dd * ff == "-45258849200337190631492857400003938881995610529251881450243326128168934937055005474972396281351684800":bigint);
  assert(aa * bb * cc * dd * ff * gg == "45258849200337190631492857400003938881995610529251881450243326128168934937055005474972396281351684800":bigint);
  

  // Bit shifts
  assert(5:bigint  << 3   == 40);
  assert(5:bigint  >> 1   == 2);
  assert(5:bigint  >> true== 2);
  assert(5:bigint  << true== 10);
  assert(-5:bigint << 3   == -40);
  assert(-5:bigint >> 1   == -3);
  assert(5:bigint  >> -3  == 40);
  assert(5:bigint  << -1  == 2);
  assert(-5:bigint >> -3  == -40);
  assert(-5:bigint << -1  == -3);

  assert(5:bigint  << 3:uint  == 40);
  assert(5:bigint  >> 1:uint  == 2);
  assert(-5:bigint << 3:uint  == -40);
  assert(-5:bigint >> 1:uint  == -3);

  // assign ops
  s = 5:bigint;
  s <<= 1;
  assert(s == 10:bigint);
  s = 5:bigint;
  s >>= 1;
  assert(s == 2:bigint);
  s = 5:bigint;
  s <<= (true);
  assert(s == 10:bigint);
  s = 5:bigint;
  s >>= (true);
  assert(s == 2:bigint);

  // right shifting a negative value over its size will always result in -1,
  // not 0
  var neg = -5:bigint;
  neg >>= 64;
  assert(neg == -1);
  neg = -5; 
  assert(neg >> 64 == -1);

  // Boolean ops
  assert(~123:bigint == -124);
  assert(123:bigint & 234:bigint == 106);
  assert(123:bigint | 234:bigint == 251);
  assert(123:bigint ^ 234:bigint == 145);

  s = 17:bigint;
  s &= 13:bigint;
  assert(s == 1:bigint);
  s = 17:bigint;
  s |= 13:bigint;
  assert(s == 29:bigint);
  s = 17:bigint;
  s ^= 13:bigint;
  assert(s == 28:bigint);

  var ret:bigint;
  gcd(ret, 48:bigint, 180:bigint);
  assert(ret == 12);
  lcm(ret, 48:bigint, 180:bigint);
  assert(ret == 720);

  fac(ret, 40);
  assert(ret == "815915283247897734345611269596115894272000000000":bigint);

  xor(ret, la,lb);
  assert(ret == 327299);
  assert(la & lb == 124);
  assert(la | lb == 327423);

  assert((90:bigint).sizeInBase(10) == 3);
  assert((99:bigint).sizeInBase(10) == 3);

  sqrt(ret, 4:bigint);
  assert(ret == 2);
  sqrt(ret, 5:bigint);
  assert(ret == 2);

  assert((6:bigint)%(5:bigint) == 6%5);

  // Conversions
  use Random;
  import ChplConfig;
  param intSize = if ChplConfig.CHPL_TARGET_PLATFORM == 32 then 32 else 64;
  var randStream = new randomStream(int(intSize));
  var randVal = randStream.next();
  assert(randVal:bigint == randVal);

  var uRandStream = new randomStream(uint(intSize));
  var uRandVal = uRandStream.next();
  assert(uRandVal:bigint == uRandVal);

  assert(a.cmp(b) == -1);
  assert(b.cmp(a) == 1);
  assert(a.cmp(a) == 0);

  // cmps
  assert(a == a);
  assert(a < b);
  assert(a <= b);
  assert(b > a);
  assert(b >= a);
  assert(a != b);

  s = 17:bigint;
  assert(s == 17:int);
  assert(s >  16:int);
  assert(s >= 16:int);
  assert(s <  18:int);
  assert(s <= 18:int);
  assert(s != 18:int);
  assert(s == 17:uint);
  assert(s >  16:uint);
  assert(s >= 16:uint);
  assert(s <  18:uint);
  assert(s <= 18:uint);
  assert(s != 18:uint);
  assert(17:int  == s);
  assert(16:int  <  s);
  assert(16:int  <= s);
  assert(18:int  >  s);
  assert(18:int  >= s);
  assert(18:int  != s);
  assert(17:uint == s);
  assert(16:uint <  s);
  assert(16:uint <= s);
  assert(18:uint >  s);
  assert(18:uint >= s);
  assert(18:uint != s);

  s = 1:bigint;
  assert(s == (true));
  assert(s >  (false));
  assert(s >= (false));
  s = 0:bigint;
  assert(s <  (true));
  assert(s <= (true));
  assert(s != (true));
  s = 1:bigint;
  assert((true)  == s);
  assert((false) <  s);
  assert((false) <= s);
  s = 0:bigint;
  assert((true) >  s);
  assert((true) >= s);
  assert((true) != s);

  // test comparison under promotion
  var abc = [a, b, c];
  assert(abc == [a, b, c]);
  assert(abc > [1,2,3]);
  assert(abc > 1);
  assert(17:bigint > [1,2,3]);
  var ss = [1:bigint, 2:bigint, 3:bigint];
  assert(ss == [1,2,3]);


}
