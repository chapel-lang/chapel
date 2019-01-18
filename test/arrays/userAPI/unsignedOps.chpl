use arrayAPItest;

{
  var A: [1:uint..4:uint, 1:uint..4:uint] real;

  testArrayAPI2D("Array with unsigned domain", A, {2:uint..3:uint, 3:uint..4:uint}, {0:uint..3:uint, 1:uint..8:uint by 2:uint});
}

{
  var A: [0:uint..5:uint, 0:uint..5:uint] real;

  testArrayAPI2D("Slice of array with unsigned domain", A[1:uint..4:uint, 1:uint..4:uint], {2:uint..3:uint, 3:uint..4:uint}, {0:uint..3:uint, 1:uint..8:uint by 2:uint});
}

{
  var A: [1:uint..4:uint, 1:uint..4:uint] real;

  testArrayAPI2D("Reindex of array with unsigned domain", A.reindex( 0:uint..3:uint, 2:uint..9:uint by 2:uint) , {1:uint..2:uint, 6:uint..9:uint by 2:uint}, {1:uint..8:uint by 2:uint, 2:uint..5:uint});
}

{
  var A: [0:uint..5:uint, 1:uint..6:uint, 1:uint..4:uint] real;

  testArrayAPI2D("RankChange of array with unsigned domain", A[1:uint..4:uint, 1:uint..4:uint, 2:uint], {2:uint..3:uint, 3:uint..4:uint}, {0:uint..3:uint, 1:uint..8:uint by 2:uint});
}
