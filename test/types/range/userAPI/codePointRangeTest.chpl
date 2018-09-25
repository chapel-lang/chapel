use rangeAPItest;

testRangeAPI("codepoint range", 1:codePointIndex..10:codePointIndex,
             4:codePointIndex, 3:codePointIndex..6:codePointIndex);
testRangeAPI("codepoint range", 1:codePointIndex..20:codePointIndex by 2,
             7:codePointIndex, 5:codePointIndex..13:codePointIndex by 2);
