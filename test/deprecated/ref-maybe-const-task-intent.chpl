var y: [1..10] int;
coforall i in 1..10 {
  y[i] = i;
}

var z: [1..10] int;
cobegin {
  z[1] = 1;
  z[2] = 2;
}

var w: [1..10] int;
sync {
  begin {
    w[1] = 1;
  }
}
