var s = "123";

var x = s:int;

writeln("x is: ", x);

var f = s:real;

writeln("f is: ", f);

s = "true";

var b = s:bool;

writeln("b is: ", b);

s = "1.2i";

var im = s:imag;

writeln("im is: ", im);

writeln("x.numBits = ", numBits(x.type));
writeln("f.numBits = ", numBits(f.type));
writeln("im.numBits = ", numBits(im.type));
