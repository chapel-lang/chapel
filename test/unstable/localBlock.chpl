
var x: int = 0;
proc times2(x){
  return x * 2;
}
local do
  x = 5;

local {
  x = times2(x);
  x = times2(x);
}

local x.locale == here {
  x = times2(x);
  x = times2(x);
}

local do on here {
  writeln("On here locale ", here);
}

