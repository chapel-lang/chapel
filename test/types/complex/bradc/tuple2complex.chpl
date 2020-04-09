var x = 1.2: real(32),
    y = 3.4: real(32);

type mycomplex = complex,
     mycomplex32 = complex(64),
     mycomplex64 = complex(128);

var c1 = (x, y): complex;
writeln("c1 is: ", c1);

var c2 = (x,y): complex(128);
writeln("c2 is: ", c2);

var c3 = (x,y): complex(64);
writeln("c3 is: ", c3);

var c4 = (x,y): mycomplex;
writeln("c4 is: ", c4);

var c5 = (x,y): mycomplex32;
writeln("c5 is: ", c5);

var c6 = (x,y): mycomplex64;
writeln("c6 is: ", c6);
