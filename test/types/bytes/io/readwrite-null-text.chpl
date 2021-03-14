use IO;

var b: bytes;
readln(b);
if b.size != 3 then
  halt("Incorrect length for the bytes object");
write(b);
