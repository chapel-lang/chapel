var realL = 1_2.3_4e5;
var realS = "1_2.3_4e5": real;

var imagL = 1_2.3_4e5i;
var imagS = "1_2.3_4e5i": imag;

var hexRealL = 0x1_2.3_4e5;
var hexRealS = "0x1_2.3_4e5": real;

var hexImagL = 0x1_2.3_4e5i;
var hexImagS = "0x1_2.3_4e5i": imag;

assert(realL == realS);
assert(imagL == imagS);
assert(hexRealL == hexRealS);
assert(hexImagL == hexImagS);
