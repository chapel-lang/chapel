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
writeln(s[..4:byteIndex]);
writeln(s[6:byteIndex..]);
writeln(s[3:byteIndex..7:byteIndex]);
writeln();

// with stride
writeln(s[0:byteIndex..10:byteIndex by 2]);
writeln(s[0:byteIndex..10:byteIndex by -1]);
writeln(s[0:byteIndex..10:byteIndex by -2]);
writeln(s[0:byteIndex.. by 3]);
writeln(s[1:byteIndex.. by 3]);
writeln();

// string.this(uint)
writeln(s[2:byteIndex]);
writeln();

// all together now!
writeln(s[0:byteIndex..s.find(" ")],
        "beautifu", s[2:byteIndex],
        s[s.find(" ")..]);
