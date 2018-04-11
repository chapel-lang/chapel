enum numbers {
  one = 1,
  two = 2,
  three = one:int + two:int
};

for n in numbers do
  writeln(n:int);
