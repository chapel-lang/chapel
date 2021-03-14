require "chplu01.h", "chplu01.c", "-ltestu01", "-lprobdist", "-lmylib";

config param testReal = true;
config param useNPB = false;
config param testRealBits = 64;
config param realTruncateBits = 0;

config param testBoundedRand = false;
config param testUintBits = 64;

config const seedi = 1;
config const seed = getSeed(seedi);
config const crush = false;

use Random;

proc getSeed(i:int) {
  var seeds = [1, 589494289, 11, 31415, 2415065787, 111111,
               589494289+1000, 520326001786743721, 54553810781164123,
               845796262492190295, 944310181042360313,
               7, 17, 314159265, 271828182845, 0xfffffff ];
  return seeds(i);
}

proc getRNG() {
  if useNPB {
    // NPB
    if testRealBits == 64 then
      return createRandomStream(seed, eltType=real(64), parSafe=false, algorithm=RNG.NPB);
    else if testRealBits == 32 then
      return createRandomStream(seed, eltType=real(64), parSafe=false, algorithm=RNG.NPB);
  } else {
    // PCG
    if testReal && realTruncateBits == 0 {
      // Make native RNG with eltType=real(size)
      if testRealBits == 64 then
        return createRandomStream(seed, eltType=real(64), parSafe=false, algorithm=RNG.PCG);
      else if testRealBits == 32 then
        return createRandomStream(seed, eltType=real(32), parSafe=false, algorithm=RNG.PCG);
    } else {
      // Make an integer RNG
      if testUintBits == 64 then
        return createRandomStream(seed, eltType=uint(64), parSafe=false, algorithm=RNG.PCG);
      else if testUintBits == 32 then
        return createRandomStream(seed, eltType=uint(32), parSafe=false, algorithm=RNG.PCG);
    }
  }
}

var rs = getRNG();

export proc test_rand_double():real {
  var tmp = rs.getNext();
  if tmp.type == real(64) || tmp.type == real(32) {
    if testRealBits == 64 then
      return tmp;
    else if testRealBits == 32 then
      return tmp:real(32);
  } else {
    var x = tmp:uint;
    if realTruncateBits == 64 {
      return ldexp(x, -64);
    } else {
      const mask = 1:uint << realTruncateBits - 1;
      const n = x & mask;
      return ldexp(n, -realTruncateBits);
    }
  }
}

var uint_part_i = 0;
var uint_part_v:uint = 0;
config param mask32bits = 0xffffffff;
assert(mask32bits == (1 << 32) - 1);

var tmprng:pcg_setseq_64_xsh_rr_32_rng;
var tmpi = 0;
var tmpnext:uint(32) = 0;

export proc test_rand_uint32():c_uint {
  if !testBoundedRand {
    if testUintBits == 64 {
      var ret:uint(32);
      if uint_part_i == 0 {
        uint_part_v = rs.getNext():uint;
        // return the high-order bits first.
        ret = ((uint_part_v >> 32) & mask32bits):uint(32);
      } else {
        // return the low-order bits second
        ret = (uint_part_v & mask32bits):uint(32);
      }
      uint_part_i ^= 1;
      return ret;
    } else if testUintBits == 32 {
      return rs.getNext():uint(32);
    } else {
      halt("Bad settings for test_rand_uint32");
    }
  } else {
    // Test bounded rand.
    // Generate a number in 0..2^31+2^30.
    // Then left shift 1 and take the top 16 bits.
    // Do that a 2nd time, combining the 16-bit values.
    // The result is a 32-bit value that should show bias
    // if the bounded rand strategy is naive.
    
    // E.g. 2^32 + 2^30 being max ->
    // if you just did a mod by that, the 32-bit numbers divide into 4 parts:
    //   0..2^30
    //   2^30..2^31
    //   2^31..2^31+2^30
    //   ..2^32
    //
    // and the last part would mod-map down to the 1st, giving a skewed distribution.

    if testUintBits != 32 then compilerError("Bad configuration");

    var max:uint(32) = (2**31 + 2**30 + 1):uint(32);
    var two_31:uint(32) = 2**31;

    // Get a number <= 2^31
    var num1, num2:uint(32);
    while true {
      num1 = rs.getNext(0, max);
      if num1 < two_31 then break;
    }

    while true {
      num2 = rs.getNext(0, max);
      if num2 < two_31 then break;
    }

    // Now we have random 31-bit numbers, generated in a curious manner

    // Ignore their top bits since they are always 0.
    num1 <<= 1;
    num2 <<= 1;
    // Mask each by their top 16 bits.
    num1 &= 0xffff0000;
    num2 &= 0xffff0000;

    // Combine the top 16 bits from each number to make a 32-bit number.
    return num1 ^ (num2 >> 16);
  }
}

extern proc run_smallcrush_testu01_double(name:c_string);
extern proc run_smallcrush_testu01_uint(name:c_string);
extern proc run_crush_testu01_double(name:c_string);
extern proc run_crush_testu01_uint(name:c_string);

var name:string;

if useNPB then
  name += "NPB";
else
  name += "PCG";

if testReal {
  name += "real" + testRealBits:string;
}

if testReal && !useNPB && realTruncateBits != 0 {
  name += "truncate" + realTruncateBits:string;
}
if !testReal {
  name += testUintBits;
  if testBoundedRand then
    name += "bounded";
}

writeln(name, " with seed ", seed, " using RandomStream type:");
writeln(rs.type:string);
writeln("RNG:");
writeln(rs);

if testReal {
  writeln("testing real RNG");
  for i in 1..10 {
    writeln(test_rand_double());
  }
  // start over, don't count above as run-up
  rs = getRNG();
  if crush then
    run_crush_testu01_double(name.localize().c_str());
  else
    run_smallcrush_testu01_double(name.localize().c_str());
} else {
  writeln("testing uint ", testUintBits, " RNG");
  for i in 1..10 {
    writef("%xu\n", test_rand_uint32());
  }
  // start over, don't count above as run-up
  rs = getRNG();
  if crush then
    run_crush_testu01_uint(name.localize().c_str());
  else
    run_smallcrush_testu01_uint(name.localize().c_str());
}


