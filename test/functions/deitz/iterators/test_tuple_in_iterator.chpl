iter foo() {
  var c: 2*int;
  while c(2) < 3 {
    //c = c;
    yield c;
    if c(1) < 2 then
      c(1) = c(1) + 1;
    else {
      c(1) = 0;
      c(2) = c(2) + 1;
    }
  }
}

for i in foo() do
  writeln(i);
