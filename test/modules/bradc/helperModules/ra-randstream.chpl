module RARandomStream {
  param randWidth = 64;
  type randType = uint(randWidth);

  // BLC: would eventually like these to be params as well
  const bitDom = [0..#randWidth],
        m2: [bitDom] randType = computeM2Vals(randWidth);


  iter RAStream(numvals, start = 0:randType): randType {
    var val = getNthRandom(start);
    for i in 1..numvals {
      getNextRand(val);
      yield val;
    }
  }


  proc getNthRandom(in n) {
    param period = 0x7fffffffffffffff/7;
    
    n %= period;
    if (n == 0) then return 0x1;

    var ran: randType = 0x2;
    for i in 0..#log2(n) by -1 {
      var val: randType = 0;
      for j in bitDom do
        if ((ran >> j) & 1) then val ^= m2(j);
      ran = val;
      if ((n >> i) & 1) then getNextRand(ran);
    }
    return ran;
  }


  // BLC: would also like to see this fn inlined -- how to specify?
  proc getNextRand(inout x) {
    param POLY:randType = 0x7;  // BLC: this cast should be unnecessary?
    param hiRandBit = 0x1:randType << (randWidth-1);

    x = (x << 1) ^ (if (x & hiRandBit) then POLY else 0);
  }


  iter computeM2Vals(numVals): randType {
    var nextVal = 0x1: randType;
    for i in 1..numVals {
      yield nextVal;
      getNextRand(nextVal);
      getNextRand(nextVal);
    }
  }
}

