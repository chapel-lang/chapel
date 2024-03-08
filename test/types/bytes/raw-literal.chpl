use IO;
var b1 = b"""some\nbytes, Here are 'some' more escapes:
            \t \r \xff This literal uses double quotes""";
writeln("Value: ", b1);
writeln("Type: ", b1.type:string);
stdout.writeBinary(b1, b1.size);
writeln();

var b2 = b'''some\nbytes, Here are "some" more escapes:
            \t \r \xff This literal uses single quotes''';
writeln("Value: ", b2);
writeln("Type: ", b2.type:string);
stdout.writeBinary(b2, b2.size);
writeln();
