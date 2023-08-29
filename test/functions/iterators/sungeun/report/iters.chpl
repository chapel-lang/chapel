config const n = 7;
config const off = n/2;

var initial = 0;

proc initMe() {
  initial += off;
  return initial;
}

iter i0() {
  for i in 1..n do
    yield i;
}

iter i1() {
  const x = initMe();
  for i in 1..n {
    yield x+i;
  }
}

iter i2() {
  var x = initMe();
  for i in 1..n {
    x += i;
    yield x;
  }
}

iter i3() {
  var x = initMe();
  for i in 1..n {
    x += i;
    yield x;
    x -= i;
  }
}

iter i4() {
  var x = initMe();
  for i in 1..n {
    x += i;
    yield x;
    x -= i;
  }
  initial = 4;
}

iter i5() {
  var x = initMe();
  for i in 1..n {
    x += i;
    yield x;
  }
  initial = 5;
}

iter i6() {
  const x = initMe();
  for i in 1..n {
    yield x+i;
  }
  initial = 6;
}

iter i7() {
  for i in 1..n {
    yield i;
  }
  initial = 7;
}
