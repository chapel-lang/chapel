config const n = 7;
config const off = n/2;

var init = 0;

proc initMe() {
  init += off;
  return init;
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
  init = 4;
}

iter i5() {
  var x = initMe();
  for i in 1..n {
    x += i;
    yield x;
  }
  init = 5;
}

iter i6() {
  const x = initMe();
  for i in 1..n {
    yield x+i;
  }
  init = 6;
}

iter i7() {
  for i in 1..n {
    yield i;
  }
  init = 7;
}
