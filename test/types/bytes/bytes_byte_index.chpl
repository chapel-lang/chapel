// this is the bytes version of types/string/bwross/string_byte_index.chpl
var s:bytes = "hello world";

// bytes.find
write(s.find(" ":bytes), " ");
write(s.find("hello":bytes), " ");
write(s.find("world":bytes), " ");
write(s.find("nowhere":bytes), " ");
write(s.find(s), " ":bytes);
writeln(s.find("":bytes));
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
writeln(s[1..s.find(" ":bytes)], "beautifu", s.item(3), s[s.find(" ":bytes)..]);
