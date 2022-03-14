proc countDown(x: int, ref pos = new int(0)) {
  if x == 0 then
    writeln(pos);
  else {
    pos += x;
    writeln("at ", x, ":", pos);
    countDown(x-1, pos);
  }
}

countDown(10);
