enum numbers {
  one = 1,
  two,
  x = 0x8000000000000000,
  three = one:uint + two:uint
};

writeln(numbers.one, " ", numbers.one:uint);
writeln(numbers.x, " ", numbers.x:uint);

var t:numbers = numbers.three;
writeln(t, " ", t:uint);
