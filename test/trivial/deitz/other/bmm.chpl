use Time, BitOps;

config var printTiming = false;

proc writeBinaryMatrix(u: uint(64)) {
  for i in 0..7 by -1 {
    for j in 0..7 by -1 {
      write((u&(1:uint(64)<<((i*8)+j)) != 0):int);
    }
    writeln();
  }
  writeln();
}

inline proc byteOrReduce(u: uint(64)) {
  var t1 = u | (u >> 4);
  var t2 = t1 | (t1 >> 2);
  var t3 = t2 | (t2 >> 1);
  return t3 & 0x0101010101010101;
}

inline proc byteExpand(u: uint(64)) {
  return 0x8080808080808080 ^ (0x8080808080808080 - u);
}

proc bitMatTrans(x: uint(64)) {
  // Return the transpose of x, treating it as an 8x8 bit-matrix.
  return   x & 0x8040201008040201        |
          (x & 0x0080402010080402) <<  7 |
          (x & 0x0000804020100804) << 14 |
          (x & 0x0000008040201008) << 21 |
          (x & 0x0000000080402010) << 28 |
          (x & 0x0000000000804020) << 35 |
          (x & 0x0000000000008040) << 42 |
          (x & 0x0000000000000080) << 49 |
          (x >>  7) & 0x0080402010080402 |
          (x >> 14) & 0x0000804020100804 |
          (x >> 21) & 0x0000008040201008 |
          (x >> 28) & 0x0000000080402010 |
          (x >> 35) & 0x0000000000804020 |
          (x >> 42) & 0x0000000000008040 |
          (x >> 49) & 0x0000000000000080;
}

proc oldBitMatMultOr(x: uint(64), y: uint(64)): uint(64) {
  var yTranspose = bitMatTrans(y);
  var result:uint(64) = 0;
  for i in 0..7:uint(64) {
    var left = (x & (0xFF:uint(64) << (i*8))) >> (i*8);
    for param j in 0..7:uint(64) {
      if (0 != (left & ((yTranspose & (0xFF:uint(64) << (j*8))) >> (j*8)))) then
        result |= 1:uint(64) << (i*8 + j):uint(64);
    }
  }
  return result;
}

proc newBitMatMultOr(x: uint(64), y: uint(64)): uint(64) {
  var result:uint(64) = 0;
  var yTranspose = bitMatTrans(y);
  result |= byteExpand(byteOrReduce(x & yTranspose)) & 0x8040201008040201;
  yTranspose = rotl(yTranspose, 8);
  result |= byteExpand(byteOrReduce(x & yTranspose)) & 0x4020100804020180;
  yTranspose = rotl(yTranspose, 8);
  result |= byteExpand(byteOrReduce(x & yTranspose)) & 0x2010080402018040;
  yTranspose = rotl(yTranspose, 8);
  result |= byteExpand(byteOrReduce(x & yTranspose)) & 0x1008040201804020;
  yTranspose = rotl(yTranspose, 8);
  result |= byteExpand(byteOrReduce(x & yTranspose)) & 0x0804020180402010;
  yTranspose = rotl(yTranspose, 8);
  result |= byteExpand(byteOrReduce(x & yTranspose)) & 0x0402018040201008;
  yTranspose = rotl(yTranspose, 8);
  result |= byteExpand(byteOrReduce(x & yTranspose)) & 0x0201804020100804;
  yTranspose = rotl(yTranspose, 8);
  result |= byteExpand(byteOrReduce(x & yTranspose)) & 0x0180402010080402;
  return result;
}

proc main() {
  var D = {0..1000:uint(64), 0..1000:uint(64)};
  var time = new Timer();
  var x: uint(64);

  for (i,j) in D do
    if oldBitMatMultOr(i, j) != newBitMatMultOr(i, j) then
      halt("error at ", i, ", ", j);

  time.start();
  for (i, j) in D {
    x |= oldBitMatMultOr(i, j);
  }
  time.stop();
  if printTiming then writeln("old: ", time.elapsed(TimeUnits.milliseconds));
  time.clear();
  time.start();
  for (i, j) in D {
    x |= newBitMatMultOr(i, j);
  }
  time.stop();
  if printTiming then writeln("new: ", time.elapsed(TimeUnits.milliseconds));
  writeln(x);
  writeln((newBitMatMultOr(987151324, 234907813), oldBitMatMultOr(987151324, 234907813)));
}
