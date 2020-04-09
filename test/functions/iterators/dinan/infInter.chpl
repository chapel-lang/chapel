iter wraparound(n: int): int {
  while true do
    for i in 1..n do yield i;
}

var x = 0;

for i in wraparound(5) {
  writeln(i);
  x += 1;
  if x >= 20 then break;
}
