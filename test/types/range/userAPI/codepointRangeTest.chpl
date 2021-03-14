use rangeAPItest;

testRangeAPI("codepoint range", 1:codepointIndex..10:codepointIndex,
             4:codepointIndex, 3:codepointIndex..6:codepointIndex);
testRangeAPI("strided codepoint range", 1:codepointIndex..20:codepointIndex by 2,
             7:codepointIndex, 5:codepointIndex..13:codepointIndex by 2);
testRangeAPI("low bounded codepoint range", 1:codepointIndex..,
             4:codepointIndex, 3:codepointIndex..6:codepointIndex);
testRangeAPI("high bounded codepoint range", ..10:codepointIndex,
             4:codepointIndex, 3:codepointIndex..6:codepointIndex);
