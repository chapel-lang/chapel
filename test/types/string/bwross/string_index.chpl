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

// string.this(uint)
writeln(s[3]);
writeln();

// all together now!
writeln(s[1:byteIndex..s.find(" ")],
        "beautifu", s[3],
        s[s.find(" ")..]);
