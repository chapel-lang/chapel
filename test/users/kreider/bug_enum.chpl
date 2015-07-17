/* bug: Enum constants don't resolve type, raising compiler error. */

enum clrmasks {
  RED = 0xff0000, GREEN = 0x00ff00, BLUE=0x0000ff
}

var g = 0x123456 & clrmasks.GREEN;
writef("g = x0%06xu\n", g);

writef("g = 0x%06xu\n", 0x123456 & clrmasks.GREEN);

if (clrmasks.BLUE == 0xff) then writeln("blue is 0xff");

/* Bit operations against a variable do work now. */

const pix = 0x123456;
var pixg : int;
pixg = 0x654321;
writef("g = 0x%06xu\n", pixg & clrmasks.GREEN);






