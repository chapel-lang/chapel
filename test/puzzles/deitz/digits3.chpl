config param digits = 4;

def number2tuple(n: int) {
  var nn: digits*int;
  for param i in 1..digits do
    nn(i) = n/10**(digits-i)%10;
  return nn;
}

def tuple2number(nn: digits*int) {
  var n: int;
  for param i in 1..digits do
    n += nn(i)*10**(digits-i);
  return n;
}

def smallest(n: int) {
  var nn = number2tuple(n);
  for param i in 1..digits-1 do
    for param j in 1..digits-1 do
      if nn(j) > nn(j+1) then
        nn(j) <=> nn(j+1);
  return tuple2number(nn);
}

def biggest(n: int) {
  var nn = number2tuple(n);
  for param i in 1..digits-1 do
    for param j in 1..digits-1 do
      if nn(j) < nn(j+1) then
        nn(j) <=> nn(j+1);
  return tuple2number(nn);
}

def sortedNumbers(param p: int = 1, start: int = 0) {
  if p == digits {
    for i in max(start,1)..9 do
      yield i;
  } else {
    for i in start..9 do
      for j in sortedNumbers(p+1, i) do
        yield i*10**(digits-p)+j;
  }
}

var D: domain(int);
var FD: domain(int);

for i in sortedNumbers() {
  writeln(i);
  for j in D do {
writeln("----", j);
    D -= j;
  }
  var j = i;
  while !D.member(j) {
writeln("--", j);
    D += j;
    j = biggest(j) - smallest(j);
  }
  if !FD.member(j) {
    while !FD.member(j) {
      FD += j;
      writeln(j, " - ");
      j = biggest(j) - smallest(j);
    }
    writeln();
  }
}
