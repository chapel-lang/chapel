var realL = 12.34e5;
var realS = "12.34e5": real;

var imagL = 12.34e5i;
var imagS = "12.34e5i": imag;

var hexRealL = 0x12.34e5;
var hexRealS = "0x12.34e5": real;

var hexImagL = 0x12.34e5i;
var hexImagS = "0x12.34e5i": imag;

assert(realL == realS);
assert(imagL == imagS);
assert(hexRealL == hexRealS);
assert(hexImagL == hexImagS);
