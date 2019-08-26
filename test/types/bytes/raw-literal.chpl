var b = b"""some\nbytes, Here are some more escapes:
            \t \r \xff""";
writeln(b);
writef("%|*s\n", b.length, b);

