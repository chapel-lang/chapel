config const cc1 = 111, cc2 = 222;

iter helper() {
  yield 1;
  yield 2;
  yield 3;
}

/////////// all yields reference the same local symbol, no loop ///////////

// local var
iter it11nl() {
  var x = cc1;
  yield x;
  x += cc2;
  yield x;
}
for va11 in it11nl() do writeln("it11nl ", va11); // 111,333

// local const (it actually propagates?)
iter it12nl() {
  const x = cc1;
  yield x;
  writeln();
  yield x;
  writeln();
}
for va12 in it12nl() do write("it12nl ", va12); // 111,111

// const formal (does it propagate?)
iter it13nl(const in arg: int) {
  write(" ");
  yield arg;
  write("\n ");
  yield arg;
  write("\n");
}
for va13 in it13nl(1313) do write("it13nl ", va13); // 1313,1313

// var formal
iter it14nl(in arg: int) {
  write(" ");
  yield arg;
  write("\n ");
  arg += cc1;
  yield arg;
  write("\n");
}
for va14 in it14nl(1414) do write("it14nl ", va14); // 1414,1525

/////////// all yields reference the same local symbol, in a loop ///////////

// local var
iter it11yl() {
  for lv11 in helper() {
    var x = cc1;
    yield x;
    x += cc2;
    yield x;
  }
}
for va11 in it11yl() do writeln("it11yl ", va11); // 111,333

// local const (it actually propagates?)
iter it12yl() {
  for lv12 in helper() {
    const x = cc1;
    yield x;
    writeln();
    yield x;
    writeln();
  }
}
for va12 in it12yl() do write("it12yl ", va12); // 111,111

// const formal (does it propagate?)
iter it13yl(const in arg: int) {
  for lv13 in helper() {
    write(" ");
    yield arg;
    write("\n ");
    yield arg;
    write("\n");
  }
}
for va13 in it13yl(1313) do write("it13yl ", va13); // 1313,1313

// var formal
iter it14yl(in arg: int) {
  for lv14 in helper() {
    write(" ");
    yield arg;
    write("\n ");
    arg += cc1;
    yield arg;
    write("\n");
  }
}
for va14 in it14yl(1414) do write("it14yl ", va14); // 1414,1525,1525,1636,1636,1747

// loop induction variable
iter it10yl() {
  for lv10 in helper() {
    yield lv10;
    writeln();
    yield lv10;
    writeln();
  }
}
for va10 in it10yl() do write("it10yl ", va10); // 1,1,2,2,3,3
