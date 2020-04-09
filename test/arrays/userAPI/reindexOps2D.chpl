use arrayAPItest;

var A: [1..4, 1..4] real;

testArrayAPI2D("array reindex", A.reindex(0..3, 2..9 by 2), {1..2, 6..9 by 2}, {1..8 by 2, 2..5});
