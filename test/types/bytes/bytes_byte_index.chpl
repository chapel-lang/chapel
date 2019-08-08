// this is the bytes version of types/string/bwross/string_byte_index.chpl
var s:_bytes = "hello world";

// bytes.find
write(s.find(" ":_bytes), " ");
write(s.find("hello":_bytes), " ");
write(s.find("world":_bytes), " ");
write(s.find("nowhere":_bytes), " ");
write(s.find(s), " ":_bytes);
writeln(s.find("":_bytes));
writeln();

// bytes.this(range)
writeln(s[..]);
writeln(s[..5]);
writeln(s[7..]);
writeln(s[4..8]);
writeln();

// with stride
writeln(s[1..11 by 2]);
writeln(s[1..11 by -1]);
writeln(s[1..11 by -2]);
writeln(s[1.. by 3]);
writeln(s[2.. by 3]);
writeln();

// bytes.this(uint)
writeln(s[3]);
writeln();

// all together now!
writeln(s[1..s.find(" ":_bytes)], "beautifu", s[3], s[s.find(" ":_bytes)..]);
