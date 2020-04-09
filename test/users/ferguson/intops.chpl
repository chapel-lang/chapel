proc test(type t)
{
  var x:t;
  x = 0;
  x += 1;

  var y:t;
  for i in 0..x {
    y += i;
  }

  return y;
}

writeln(test(int));
writeln(test(uint));
writeln(test(int(8)));
writeln(test(uint(8)));
writeln(test(int(16)));
writeln(test(uint(16)));
writeln(test(int(32)));
writeln(test(uint(32)));
writeln(test(int(64)));
writeln(test(uint(64)));

