param randWidth = 64;
type randType = uint(randWidth);


proc getNextRand(inout x) {
  param POLY = 0x7;
  param hiRandBit = 0x1:randType << (randWidth-1);

  x = (x << 1) ^ (if (x & hiRandBit) then POLY else 0);
}

var x = 1:randType;
writeln("x is: ", x);
getNextRand(x);
writeln("x is: ", x);
getNextRand(x);
writeln("x is: ", x);
