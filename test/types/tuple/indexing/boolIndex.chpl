config param errors = false;
config const coinflip = false;

var t = (3.3, 4.4);
var t2 = ("hi", 46);

writeln(t(coinflip));
writeln(t(true));
writeln(t(false));

if errors then
  writeln(t2(coinflip));
writeln(t2(true));
writeln(t2(false));
