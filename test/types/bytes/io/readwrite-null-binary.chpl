var b: bytes;
readf("%|3s", b);
if b.length != 3 then
  halt("Incorrect length for the bytes object");
writef("%|3s", b);
