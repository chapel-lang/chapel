var b  : bool     = true;
var b8 : bool(8)  = true;
var b16: bool(16) = true;
var b32: bool(32) = true;
var b64: bool(64) = true;

printAll();

b = false;
b8 = b;
b16 = b;
b32 = b;
b64 = b;

printAll();

b8 = true;
b = b8;
b16 = b8;
b32 = b8;
b64 = b8;

printAll();

b16 = false;
b = b16;
b8 = b16;
b32 = b16;
b64 = b16;

printAll();

b32 = true;
b = b32;
b8 = b32;
b16 = b32;
b64 = b32;

printAll();

b64 = false;
b = b64;
b8 = b64;
b16 = b64;
b32 = b64;

printAll();

proc printAll() {
  writeln("b is: ", b);
  writeln("b8 is: ", b8);
  writeln("b16 is: ", b16);
  writeln("b32 is: ", b32);
  writeln("b64 is: ", b64);
  writeln();
}

