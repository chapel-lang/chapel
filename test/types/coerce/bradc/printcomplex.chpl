var c1: complex = 1.2 + 3.4i;
var c2: complex = 5.6 - 7.8i;
var c3: complex = -9.0 + 1.2i;
var c4: complex = -3.4 - 5.6i;

var s: string;

s = c1:string;
writeln("s is: ", s);

s = c2:string;
writeln("s is: ", s);

s = c3:string;
writeln("s is: ", s);

s = c4:string;
writeln("s is: ", s);

var c5: complex(64) = (1.2 + 3.4i):complex(64);
var c6: complex(64) = (5.6 - 7.8i):complex(64);
var c7: complex(64) = (-9.0 + 1.2i):complex(64);
var c8: complex(64) = (-3.4 - 5.6i):complex(64);

s = c5:string;
writeln("s is: ", s);

s = c6:string;
writeln("s is: ", s);

s = c7:string;
writeln("s is: ", s);

s = c8:string;
writeln("s is: ", s);


var c9: complex(128) = 1.2 + 3.4i;
var ca: complex(128) = 5.6 - 7.8i;
var cb: complex(128) = -9.0 + 1.2i;
var cc: complex(128) = -3.4 - 5.6i;

s = c9:string;
writeln("s is: ", s);

s = ca:string;
writeln("s is: ", s);

s = cb:string;
writeln("s is: ", s);

s = cc:string;
writeln("s is: ", s);
