iter foo() {
  var c: 2*int;
  while c(1) < 3 {
    //c = c;
    yield c;
    if c(0) < 2 then
      c(0) = c(0) + 1;
    else {
      c(0) = 0;
      c(1) = c(1) + 1;
    }
  }
}

for i in foo() do
  writeln(i);
