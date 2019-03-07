config param digits = 4;

proc number2tuple(n: int) {
  var nn: digits*int;
  for param i in 1..digits do
    nn(i) = n/10**(digits-i)%10;
  return nn;
}

proc tuple2number(nn: digits*int) {
  var n: int;
  for param i in 1..digits do
    n += nn(i)*10**(digits-i);
  return n;
}

proc smallest(n: int) {
  var nn = number2tuple(n);
  for param i in 1..digits-1 do
    for param j in 1..digits-1 do
      if nn(j) > nn(j+1) then
        nn(j) <=> nn(j+1);
  return tuple2number(nn);
}

proc biggest(n: int) {
  var nn = number2tuple(n);
  for param i in 1..digits-1 do
    for param j in 1..digits-1 do
      if nn(j) < nn(j+1) then
        nn(j) <=> nn(j+1);
  return tuple2number(nn);
}

iter sortedNumbers(param p: int = 1, start: int = 0) {
  if p == digits {
    for i in max(start,1)..9 do
      yield i;
  } else {
    for i in start..9 do
      for j in sortedNumbers(p+1, i) do
        yield i*10**(digits-p)+j;
  }
}

var FD: domain(int);

for i in sortedNumbers() {
  var D: domain(int);
  var j = i;
  while !D.contains(j) {
    D += j;
    j = biggest(j) - smallest(j);
  }
  if !FD.contains(j) {
    while !FD.contains(j) {
      FD += j;
      write(j, " ");
      j = biggest(j) - smallest(j);
    }
    writeln();
  }
}
