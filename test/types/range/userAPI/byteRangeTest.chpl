use rangeAPItest;

testRangeAPI("byte index range", 1:byteIndex..10:byteIndex,
             4:byteIndex, 3:byteIndex..6:byteIndex);
testRangeAPI("strided byte index range", 1:byteIndex..20:byteIndex by 2,
             7:byteIndex, 5:byteIndex..13:byteIndex by 2);
testRangeAPI("low bounded byte index range", 1:byteIndex..,
             4:byteIndex, 3:byteIndex..6:byteIndex);
testRangeAPI("high bounded byte index range", ..10:byteIndex,
             4:byteIndex, 3:byteIndex..6:byteIndex);
