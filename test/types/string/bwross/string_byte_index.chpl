var s:string = "hello world";

// string.find
write(s.find(" "), " ");
write(s.find("hello"), " ");
write(s.find("world"), " ");
write(s.find("nowhere"), " ");
write(s.find(s), " ");
writeln(s.find(""));
writeln();

// string.this(range)
writeln(s[..]);
writeln(s[..5:byteIndex]);
writeln(s[7:byteIndex..]);
writeln(s[4:byteIndex..8:byteIndex]);
writeln();

// with stride
writeln(s[1:byteIndex..11:byteIndex by 2]);
writeln(s[1:byteIndex..11:byteIndex by -1]);
writeln(s[1:byteIndex..11:byteIndex by -2]);
writeln(s[1:byteIndex.. by 3]);
writeln(s[2:byteIndex.. by 3]);
writeln();

// string.this(uint)
writeln(s[3:byteIndex]);
writeln();

// all together now!
writeln(s[1:byteIndex..s.find(" ")],
        "beautifu", s[3:byteIndex],
        s[s.find(" ")..]);
