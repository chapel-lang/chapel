use arrayAPItest;

var A: [0..5, 0..5] real;

testArrayAPI2D("array slice", A[1..4, 1..4], {2..3, 3..4}, {0..3, 1..8 by 2});
