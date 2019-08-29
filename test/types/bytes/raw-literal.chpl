var b = b"""some\nbytes, Here are some more escapes:
            \t \r \xff""";
writeln("Value: ", b);
writeln("Type: ", b.type:string);
writef("%|*s\n", b.length, b);

