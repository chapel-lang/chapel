proc addOne(n) { // n is a parameter
  return n + 1;
}

writeln(addOne(10));

proc fibonacci(n: int): int {
  if n <= 1 then return n;
  return fibonacci(n-1) + fibonacci(n-2);
}

writeln(fibonacci(10));

proc maxOf(x ...?k) { // take a tuple of one type with k elements
  var maximum = x[1];
  for i in 2..k do maximum = if maximum < x[i] then x[i] else maximum;
  return maximum;
}

writeln(maxOf(1, -5, 123, 85, -17 , 3));

proc returnTuple(x: int, y: real = 3.1415926): (int,real) { // tuple with a default
  return (x,y);
}

writeln(returnTuple(1));
writeln(returnTuple(x=2));
writeln(returnTuple(x=-10, y=10));
writeln(returnTuple(y=-1, x=3)); // the parameters can be named out of order
