const POLY:uint(64) = 7;
var ran:uint(64) = 2;

ran = (ran << 1) ^ (if (ran:int(64) < 0) then POLY else 0);
writeln(ran);
