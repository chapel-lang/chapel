proc countDown(x: int, ref pos = 0) {
  if x == 0 then
    writeln(pos);
  else {
    pos += x;
    writeln("at ", x, ": ", pos);
    countDown(x-1, pos);
  }
}

var pos = 0;
countDown(10, pos);
