enum numbers {
  one = 1,
  two,
  x = 0x8000000000000000,
  three = one + two
};

proc is_uint(x:int) {
  writeln("not uint ", x);
}
proc is_uint(x:uint) {
  writeln("uint ", x);
}

is_uint(numbers.one);
is_uint(numbers.x);

var t:numbers = numbers.three;
is_uint(t);
