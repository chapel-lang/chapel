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
writeln(s[..4]);
writeln(s[6..]);
writeln(s[3..7]);
writeln();

// with stride
writeln(s[0..10 by 2]);
writeln(s[0..10 by -1]);
writeln(s[0..10 by -2]);
writeln(s[0.. by 3]);
writeln(s[1.. by 3]);
writeln();

// bytes.this(uint)
writeln(s[2]);
writeln();

// all together now!
writeln(s[0..s.find(" ":bytes)], "beautifu", s.item(2), s[s.find(" ":bytes)..]);
