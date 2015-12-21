enum numbers {
  one = 1,
  two = 2,
  three = one + two
};

for n in numbers.one..numbers.three do
  writeln(n);
