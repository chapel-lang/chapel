use BitOps;

config const testCase = 0;

// check edge cases of -1 and numBits(x.type)
proc test(param BW, testCaseStart=0) {
  if testCase == testCaseStart + 0 {
    var x: uint(BW) = 0x17;
    writeln(rotl(x, -1));
  } else if testCase == testCaseStart + 1 {
    var x: uint(BW) = 0x17;
    writeln(rotr(x, -1));
  } else if testCase == testCaseStart + 2 {
    var x: uint(BW) = 0x17;
    writeln(rotl(x, numBits(x.type)));
  } else if testCase == testCaseStart + 3 {
    var x: uint(BW) = 0x17;
    writeln(rotr(x, numBits(x.type)));
  } else if testCase == testCaseStart + 4 {
    var x: int(BW) = 0x17;
    writeln(rotl(x, -1));
  } else if testCase == testCaseStart + 5 {
    var x: int(BW) = 0x17;
    writeln(rotr(x, -1));
  } else if testCase == testCaseStart + 6 {
    var x: int(BW) = 0x17;
    writeln(rotl(x, numBits(x.type)));
  } else if testCase == testCaseStart + 7 {
    var x: int(BW) = 0x17;
    writeln(rotr(x, numBits(x.type)));
  }
}

test(8);
test(16, testCaseStart=8);
test(32, testCaseStart=16);
test(64, testCaseStart=24);
