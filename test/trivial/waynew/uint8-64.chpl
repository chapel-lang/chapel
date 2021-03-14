var ui:uint(64);
ui = 510;
writeln ("ui=510 is ", ui);

var ui8:uint(8);
ui8 = 128;
writeln ("ui(8)=128 is ", ui8);
ui8 = (-128):uint(8);
writeln ("ui(8)=-128 is ", ui8);
ui8 = 255; // max size of uint(8)
writeln ("ui(8)=255 is ", ui8);
ui8 = 256:uint(8);
writeln ("ui(8)=256 is ", ui8);
ui8 = 257:uint(8);
writeln ("ui(8)=257 is ", ui8);

var ui16:uint(8+8);
ui16 = 1024;
writeln ("ui(16)=1024 is ", ui16);
ui16 = 32*1024;
writeln ("ui(16)=32*1024 is ", ui16);
ui16 = (64*1024-1):uint(16); // max size for uint(16)
writeln ("ui(16)=64*1024-1 is ", ui16);
ui16 = (64*1024):uint(16);
writeln ("ui(16)=64*1024 is ", ui16);

var ui32:uint(32) = 4294967295; // max size for uint(32)
writeln ("ui(32)=4294967295 is ", ui32);
ui8 = ui32:uint(8);
writeln ("ui(8)=ui(32) is ", ui8);

var ui64:uint(64) = 18446744073709551615; // max size for uint(64)
writeln ("ui(64)=18446744073709551615 is ", ui64);

