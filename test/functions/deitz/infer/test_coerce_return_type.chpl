proc x(i: int): uint(8) {
  var x: uint(8);
  while i < 1 do
    x += 1;
  if x > 20 then
    return x;
  return 4;
}

var y = x(25);
writeln(y);
writeln(max(y.type));
