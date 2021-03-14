var b1 = b"""some\nbytes, Here are 'some' more escapes:
            \t \r \xff This literal uses double quotes""";
writeln("Value: ", b1);
writeln("Type: ", b1.type:string);
writef("%|*s\n", b1.size, b1);

var b2 = b'''some\nbytes, Here are "some" more escapes:
            \t \r \xff This literal uses single quotes''';
writeln("Value: ", b2);
writeln("Type: ", b2.type:string);
writef("%|*s\n", b2.size, b2);

