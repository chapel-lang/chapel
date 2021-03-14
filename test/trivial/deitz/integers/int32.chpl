var i1 : int(64);
i1 = 2:int(64)**31 + 1;
writeln(i1);

var i2 : int(32);
i2 = (2:int(64)**31 - 1):int(32);
writeln(i2);

var i3:int(64) = 2:int(64)**31 + 1;
writeln(i3);

var i4 = (2**31 + 1):int(32);
writeln(i4);
