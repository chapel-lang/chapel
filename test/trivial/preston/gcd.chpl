-- a beginning

function gcd(in x, in y) {
  if x < 0 then x = -x;
  if y < 0 then y = -y;

  while true {
    if x == 0 then return y;
    y = y mod x;
    if y == 0 then return x;
    x = x mod y;
  }
}


writeln(gcd(125,1005));
