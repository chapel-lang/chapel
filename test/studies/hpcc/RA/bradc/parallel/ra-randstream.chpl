module RARandomStream {
  param randWidth = 64;
  type randType = uint(randWidth);

  const bitDom = {0..#randWidth},
        m2: [bitDom] randType = computeM2Vals(randWidth);

  // TODO: Why is this still needed?
  iter RAStream() {
    var val = getNthRandom(0);
    while (1) {
      getNextRandom(val);
      yield val;
    }
  }

  iter RAStream(param tag: iterKind, followThis) where tag == iterKind.follower {
    var val = getNthRandom(followThis(1).low);
    for followThis {
      getNextRandom(val);
      yield val;
    }
  }


  proc getNthRandom(in n: uint(64)) {
    param period = 0x7fffffffffffffff/7;

    n %= period;
    if (n == 0) then return 0x1;
    var ran: randType = 0x2;
    for i in 0..log2(n)-1 by -1 {
      var val: randType = 0;
      for j in bitDom do
        if ((ran >> j) & 1) then val ^= m2(j);
      ran = val;
      if ((n >> i) & 1) then getNextRandom(ran);
    }
    return ran;
  }


  proc getNextRandom(inout x) {
    param POLY = 0x7;
    param hiRandBit = 0x1:randType << (randWidth-1);

    x = (x << 1) ^ (if (x & hiRandBit) then POLY else 0);
  }


  iter computeM2Vals(numVals) {
    var nextVal = 0x1: randType;
    for i in 1..numVals {
      yield nextVal;
      getNextRandom(nextVal);
      getNextRandom(nextVal);
    }
  }
}
