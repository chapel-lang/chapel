use arrayAPItest;

var A: [1..4, 1..4] real;

testArrayAPI2D("plain-old array", A, {2..3, 3..4}, {0..3, 1..8 by 2});
