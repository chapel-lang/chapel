var c1: complex = 1.2 + 3.4i;
var c2: complex = 5.6 - 7.8i;
var c3: complex = -9.0 + 1.2i;
var c4: complex = -3.4 - 5.6i;

var s: string;

s = c1;
writeln("s is: ", s);

s = c2;
writeln("s is: ", s);

s = c3;
writeln("s is: ", s);

s = c4;
writeln("s is: ", s);

var c5: complex(32) = (1.2 + 3.4i):complex(32);
var c6: complex(32) = (5.6 - 7.8i):complex(32);
var c7: complex(32) = (-9.0 + 1.2i):complex(32);
var c8: complex(32) = (-3.4 - 5.6i):complex(32);

s = c5;
writeln("s is: ", s);

s = c6;
writeln("s is: ", s);

s = c7;
writeln("s is: ", s);

s = c8;
writeln("s is: ", s);


var c9: complex(64) = 1.2 + 3.4i;
var ca: complex(64) = 5.6 - 7.8i;
var cb: complex(64) = -9.0 + 1.2i;
var cc: complex(64) = -3.4 - 5.6i;

s = c9;
writeln("s is: ", s);

s = ca;
writeln("s is: ", s);

s = cb;
writeln("s is: ", s);

s = cc;
writeln("s is: ", s);
