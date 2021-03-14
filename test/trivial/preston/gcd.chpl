// a beginning

proc mygcd(in x, in y) {
  if x < 0 then x = -x;
  if y < 0 then y = -y;

  while true {
    if x == 0 then return y;
    y = y % x;
    if y == 0 then return x;
    x = x % y;
  }
}


writeln(mygcd(125,1005));
