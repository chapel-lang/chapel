use arrayAPItest;

var A: [0..5, 1..6, 1..4] real;

testArrayAPI2D("rank change slice (dims 1-2)", A[1..4, 1..4, 2], {2..3, 3..4}, {0..3, 1..8 by 2});
testArrayAPI2D("rank change slice (dims 2-3)", A[5, 1..4, ..], {2..3, 3..4}, {0..3, 1..8 by 2});
testArrayAPI2D("rank change slice (dims 1&3)", A[1..4, 3, ..], {2..3, 3..4}, {0..3, 1..8 by 2});
